const express = require('express');
const cors = require('cors');
const { spawn } = require('child_process');
const path = require('path');
const readline = require('readline');
const fs = require('fs');

const app = express();
const PORT = process.env.PORT || 5000;

app.use(cors());
app.use(express.json());

const BRIDGE_EXE = path.join(__dirname, 'dsa_bridge.exe');
const DB_PATH = path.join(__dirname, 'data', 'database.json');

// -------------------------------------------------------------
// PERSISTENT C++ CORE PROCESS (IN-MEMORY RESIDENT)
// -------------------------------------------------------------
let cppProcess = null;
let cppRl = null;
const requestQueue = [];

function startCppDaemon() {
    console.log('[C++ Engine] Khởi động và nạp trước 500,000 sinh viên vào RAM...');
    cppProcess = spawn(BRIDGE_EXE, [], {
        stdio: ['pipe', 'pipe', 'inherit'],
        windowsHide: true,
    });

    cppRl = readline.createInterface({
        input: cppProcess.stdout,
        crlfDelay: Infinity
    });

    cppRl.on('line', (line) => {
        line = line.trim();
        if (!line) return;

        try {
            const parsed = JSON.parse(line);
            if (parsed.status === 'ready') {
                console.log(`[C++ Engine] Đã nạp thành công ${parsed.totalStudents.toLocaleString()} sinh viên vào RAM! Sẵn sàng phản hồi tức thì (0ms).`);
                return;
            }

            if (requestQueue.length > 0) {
                const { resolve } = requestQueue.shift();
                resolve(parsed);
            }
        } catch (err) {
            console.error('[C++ Parse Error]', err, line);
            if (requestQueue.length > 0) {
                const { reject } = requestQueue.shift();
                reject(err);
            }
        }
    });

    cppProcess.on('exit', (code) => {
        console.log(`[C++ Engine] Tiến trình thoát với code: ${code}. Đang tự động khởi động lại...`);
        while (requestQueue.length > 0) {
            const { reject } = requestQueue.shift();
            reject(new Error('C++ Process Exited'));
        }
        setTimeout(startCppDaemon, 1000);
    });
}

function sendCommandToCore(commandStr) {
    return new Promise((resolve, reject) => {
        if (!cppProcess || cppProcess.killed) {
            return reject(new Error('C++ Engine chưa sẵn sàng!'));
        }
        requestQueue.push({ resolve, reject });
        cppProcess.stdin.write(commandStr.trim() + '\n');
    });
}

// Khởi động Resident C++ Daemon ngay khi server bật
startCppDaemon();

// -------------------------------------------------------------
// SYSTEM STATS
// -------------------------------------------------------------
app.get('/api/stats', async (req, res) => {
    try {
        const data = await sendCommandToCore('stats');
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

// -------------------------------------------------------------
// DATASET GENERATOR & FAST BULK ADD (IN-MEMORY RAM)
// -------------------------------------------------------------
app.post('/api/dataset/generate', async (req, res) => {
    try {
        const { count = 500000 } = req.body;
        const data = await sendCommandToCore(`generate ${count}`);
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

app.post('/api/dataset/bulk-add', async (req, res) => {
    try {
        const { count = 50000 } = req.body;
        const data = await sendCommandToCore(`bulk_add ${count}`);
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

app.post('/api/dataset/reload', async (req, res) => {
    try {
        const data = await sendCommandToCore('reload');
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

// -------------------------------------------------------------
// MODULE 1: GPA RANGE
// -------------------------------------------------------------
app.post('/api/benchmark/gpa-range', async (req, res) => {
    try {
        const { minGpa = 8.0, maxGpa = 9.5 } = req.body;
        const data = await sendCommandToCore(`gpa_range ${minGpa} ${maxGpa}`);
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

// -------------------------------------------------------------
// MODULE 2: CLASS ID
// -------------------------------------------------------------
app.post('/api/benchmark/class-id', async (req, res) => {
    try {
        const { classId = 'C01' } = req.body;
        const data = await sendCommandToCore(`class_id ${classId}`);
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

// -------------------------------------------------------------
// MODULE 4: STUDENT ID
// -------------------------------------------------------------
app.post('/api/benchmark/student-id', async (req, res) => {
    try {
        const { studentId = '25150001' } = req.body;
        const data = await sendCommandToCore(`student_id ${studentId}`);
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

// -------------------------------------------------------------
// MODULE 5: MAX GPA
// -------------------------------------------------------------
app.post('/api/benchmark/max-gpa', async (req, res) => {
    try {
        const data = await sendCommandToCore('max_gpa');
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

// -------------------------------------------------------------
// MODULE 3: CRUD API
// -------------------------------------------------------------
app.get('/api/students', (req, res) => {
    try {
        const page = parseInt(req.query.page) || 1;
        const limit = parseInt(req.query.limit) || 15;
        const search = (req.query.search || '').trim().toLowerCase();

        if (!fs.existsSync(DB_PATH)) {
            return res.status(404).json({ status: 'error', message: 'Database file not found' });
        }

        const rawData = fs.readFileSync(DB_PATH, 'utf-8');
        const db = JSON.parse(rawData);
        let list = db.students || [];

        if (search) {
            list = list.filter(s => 
                s.id.toLowerCase().includes(search) ||
                s.name.toLowerCase().includes(search) ||
                s.classId.toLowerCase().includes(search)
            );
        }

        const total = list.length;
        const startIndex = (page - 1) * limit;
        const endIndex = startIndex + limit;
        const results = list.slice(startIndex, endIndex);

        res.json({
            status: 'success',
            total,
            page,
            limit,
            data: results
        });
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

app.post('/api/students', async (req, res) => {
    try {
        const { id, name, classId, gpa } = req.body;
        if (!id || !name || !classId || gpa === undefined) {
            return res.status(400).json({ status: 'error', message: 'Vui lòng điền đầy đủ MSSV, Họ tên, Lớp và GPA' });
        }
        const data = await sendCommandToCore(`crud_create ${id} "${name}" ${classId} ${gpa}`);
        if (data.status === 'error') {
            return res.status(400).json(data);
        }
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

app.put('/api/students/:id', async (req, res) => {
    try {
        const { id } = req.params;
        const { name, classId, gpa } = req.body;
        if (!name || !classId || gpa === undefined) {
            return res.status(400).json({ status: 'error', message: 'Vui lòng điền đầy đủ Họ tên, Lớp và GPA' });
        }
        const data = await sendCommandToCore(`crud_update ${id} "${name}" ${classId} ${gpa}`);
        if (data.status === 'error') {
            return res.status(400).json(data);
        }
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

app.delete('/api/students/:id', async (req, res) => {
    try {
        const { id } = req.params;
        const data = await sendCommandToCore(`crud_delete ${id}`);
        if (data.status === 'error') {
            return res.status(400).json(data);
        }
        res.json(data);
    } catch (err) {
        res.status(500).json({ status: 'error', message: err.message });
    }
});

app.listen(PORT, () => {
    console.log(`Backend API Server running at http://localhost:${PORT}`);
});
