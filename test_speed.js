const http = require('http');

async function testEndpoint(path, payload) {
    const start = Date.now();
    return new Promise((resolve) => {
        const req = http.request({
            hostname: 'localhost',
            port: 5000,
            path: path,
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Content-Length': Buffer.byteLength(payload)
            }
        }, (res) => {
            let data = '';
            res.on('data', chunk => data += chunk);
            res.on('end', () => {
                const duration = Date.now() - start;
                const json = JSON.parse(data);
                console.log(`[${path}] Took: ${duration} ms | Core Status: ${json.status} | Algo Time: ${json.algo1?.timeUs || json.timeUs || 0} us`);
                resolve();
            });
        });
        req.write(payload);
        req.end();
    });
}

async function run() {
    console.log('--- Testing API Latency ---');
    await testEndpoint('/api/benchmark/gpa-range', JSON.stringify({ minGpa: 8.0, maxGpa: 9.5 }));
    await testEndpoint('/api/benchmark/student-id', JSON.stringify({ studentId: '25150001' }));
    await testEndpoint('/api/benchmark/max-gpa', JSON.stringify({}));
    await testEndpoint('/api/benchmark/class-id', JSON.stringify({ classId: 'C01' }));
    console.log('--- Test Complete ---');
}

run();

