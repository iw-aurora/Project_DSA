'use client';

import React, { useState } from 'react';
import { Card, Button, Input, InputNumber, Table, Tag, App } from 'antd';
import { Zap, Search, CheckCircle2, Shuffle, XCircle } from 'lucide-react';
import axios from 'axios';

interface FinalSolutionTabProps {
  totalStudents?: number;
}

export const FinalSolutionTab: React.FC<FinalSolutionTabProps> = ({ totalStudents = 500000 }) => {
  const { message } = App.useApp();
  const [selectedTask, setSelectedTask] = useState<'mc1' | 'mc2' | 'rq1' | 'rq2'>('mc1');
  const [inputVal, setInputVal] = useState('25150001');
  const [gpaRange, setGpaRange] = useState({ min: 8.5, max: 10.0 });
  const [loading, setLoading] = useState(false);
  const [result, setResult] = useState<any>(null);

  const minId = 25150001;
  const midId = 25150000 + Math.floor(totalStudents / 2);
  const maxId = 25150000 + totalStudents;
  const nonExistId = '99999999';

  const validClasses = ['C01', 'C05', 'C08', 'C13', 'C18', 'C22', 'C25', '23DTH01', '24DTH02', '25KTP01'];
  const nonExistClass = 'C999';

  const pickRandomId = () => {
    const rand = Math.floor(Math.random() * totalStudents) + minId;
    setInputVal(String(rand));
  };

  const pickRandomClass = () => {
    const rand = validClasses[Math.floor(Math.random() * validClasses.length)];
    setInputVal(rand);
  };

  const executeFastQuery = async () => {
    setLoading(true);
    setResult(null);
    try {
      let res;
      if (selectedTask === 'mc1') {
        res = await axios.post('/api/benchmark/student-id', { studentId: inputVal });
      } else if (selectedTask === 'mc2') {
        res = await axios.post('/api/benchmark/max-gpa');
      } else if (selectedTask === 'rq1') {
        res = await axios.post('/api/benchmark/class-id', { classId: inputVal });
      } else if (selectedTask === 'rq2') {
        res = await axios.post('/api/benchmark/gpa-range', { minGpa: gpaRange.min, maxGpa: gpaRange.max });
      }
      if (res?.data?.status === 'success') {
        setResult(res.data);
      }
    } catch (err: any) {
      message.error(err.response?.data?.message || err.message || 'Lỗi truy vấn');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="flex flex-col gap-6">
      <Card className="bg-white border border-slate-200 shadow-sm rounded-xl">
        <div className="flex items-center gap-2 mb-1">
          <Zap className="w-5 h-5 text-amber-500" />
          <h2 className="text-base font-bold text-slate-800 m-0">Chế Độ Truy Vấn Tối Ưu Trực Tiếp (Production Mode)</h2>
        </div>
        <p className="text-xs text-slate-500 mb-5 m-0">
          Chỉ chạy thuật toán tối ưu tốt nhất (Hash Table O(1), Max Heap O(1) peek, Binary Search GPA) mà không in bảng so sánh Benchmark.
        </p>

        {/* Task Selection Cards */}
        <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-3 mb-6">
          <div 
            onClick={() => { setSelectedTask('mc1'); setInputVal('25150001'); setResult(null); }}
            className={`cursor-pointer p-3.5 rounded-xl border transition-all ${selectedTask === 'mc1' ? 'bg-blue-50 border-blue-500 shadow-sm ring-1 ring-blue-500' : 'bg-slate-50 border-slate-200 hover:border-slate-300'}`}
          >
            <div className="flex items-center justify-between">
              <span className="text-[10px] font-extrabold uppercase px-1.5 py-0.5 rounded bg-blue-100 text-blue-700">Bắt buộc</span>
              <span className="text-xs font-bold text-blue-600 font-mono">MC1</span>
            </div>
            <div className="text-sm font-bold text-slate-800 mt-2">Tra Cứu Theo MSSV</div>
            <div className="text-[11px] text-slate-500 mt-0.5">Exact-key lookup (Hash Table)</div>
            <div className="text-[11px] text-emerald-600 font-mono font-bold mt-1">Độ phức tạp: O(1)</div>
          </div>

          <div 
            onClick={() => { setSelectedTask('mc2'); setResult(null); }}
            className={`cursor-pointer p-3.5 rounded-xl border transition-all ${selectedTask === 'mc2' ? 'bg-amber-50 border-amber-500 shadow-sm ring-1 ring-amber-500' : 'bg-slate-50 border-slate-200 hover:border-slate-300'}`}
          >
            <div className="flex items-center justify-between">
              <span className="text-[10px] font-extrabold uppercase px-1.5 py-0.5 rounded bg-amber-100 text-amber-800">Bắt buộc</span>
              <span className="text-xs font-bold text-amber-600 font-mono">MC2</span>
            </div>
            <div className="text-sm font-bold text-slate-800 mt-2">Sinh Viên Max GPA</div>
            <div className="text-[11px] text-slate-500 mt-0.5">Extreme / Priority (Max Heap)</div>
            <div className="text-[11px] text-emerald-600 font-mono font-bold mt-1">Độ phức tạp: O(1) Peek Root</div>
          </div>

          <div 
            onClick={() => { setSelectedTask('rq1'); setInputVal('C01'); setResult(null); }}
            className={`cursor-pointer p-3.5 rounded-xl border transition-all ${selectedTask === 'rq1' ? 'bg-purple-50 border-purple-500 shadow-sm ring-1 ring-purple-500' : 'bg-slate-50 border-slate-200 hover:border-slate-300'}`}
          >
            <div className="flex items-center justify-between">
              <span className="text-[10px] font-extrabold uppercase px-1.5 py-0.5 rounded bg-purple-100 text-purple-700">Tự chọn</span>
              <span className="text-xs font-bold text-purple-600 font-mono">RQ1</span>
            </div>
            <div className="text-sm font-bold text-slate-800 mt-2">Lọc Theo Mã Lớp</div>
            <div className="text-[11px] text-slate-500 mt-0.5">Multi-result equality lookup</div>
            <div className="text-[11px] text-indigo-600 font-mono font-bold mt-1">Độ phức tạp: O(N) Indexing</div>
          </div>

          <div 
            onClick={() => { setSelectedTask('rq2'); setResult(null); }}
            className={`cursor-pointer p-3.5 rounded-xl border transition-all ${selectedTask === 'rq2' ? 'bg-emerald-50 border-emerald-500 shadow-sm ring-1 ring-emerald-500' : 'bg-slate-50 border-slate-200 hover:border-slate-300'}`}
          >
            <div className="flex items-center justify-between">
              <span className="text-[10px] font-extrabold uppercase px-1.5 py-0.5 rounded bg-emerald-100 text-emerald-700">Tự chọn</span>
              <span className="text-xs font-bold text-emerald-600 font-mono">RQ2</span>
            </div>
            <div className="text-sm font-bold text-slate-800 mt-2">Lọc Khoảng Điểm GPA</div>
            <div className="text-[11px] text-slate-500 mt-0.5">Range query (Binary Search)</div>
            <div className="text-[11px] text-emerald-600 font-mono font-bold mt-1">Độ phức tạp: O(log N + K)</div>
          </div>
        </div>

        {/* Input & Action Form */}
        <div className="flex flex-col gap-3">
          {selectedTask === 'mc1' && (
            <div className="flex flex-col gap-2">
              <div className="flex items-center gap-2">
                <span className="text-[11px] text-blue-600 bg-blue-50 border border-blue-200 px-2 py-0.5 rounded font-mono font-bold">
                  Dải MSSV hợp lệ: {minId} → {maxId.toLocaleString()}
                </span>
              </div>
              <div className="flex flex-wrap items-center gap-2">
                <Input 
                  value={inputVal} 
                  onChange={(e) => setInputVal(e.target.value)} 
                  placeholder={`Ví dụ: ${minId}`} 
                  className="w-44 font-mono font-bold text-blue-600 h-9" 
                  size="middle" 
                  prefix={<Search className="w-4 h-4 text-slate-400" />}
                />
                <Button 
                  size="middle" 
                  icon={<Shuffle className="w-3.5 h-3.5" />}
                  onClick={pickRandomId} 
                  className="text-xs font-semibold text-blue-600 bg-blue-50 border-blue-200 hover:border-blue-300 h-9"
                >
                  Ngẫu Nhiên
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(String(minId))} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Đầu dải
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(String(midId))} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Giữa dải
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(String(maxId))} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Cuối dải
                </Button>
                <Button 
                  size="middle" 
                  icon={<XCircle className="w-3.5 h-3.5" />}
                  onClick={() => setInputVal(nonExistId)} 
                  className="text-xs font-medium text-rose-600 bg-rose-50 border-rose-200 hover:border-rose-300 h-9"
                >
                  Không tồn tại
                </Button>
                <Button 
                  type="primary" 
                  size="middle" 
                  loading={loading}
                  onClick={executeFastQuery}
                  className="bg-amber-500 hover:bg-amber-400 text-white font-semibold shadow-sm ml-auto h-9 flex items-center gap-1.5"
                  icon={<Zap className="w-4 h-4" />}
                >
                  Thực Thi Thuật Toán Tối Ưu
                </Button>
              </div>
            </div>
          )}

          {selectedTask === 'mc2' && (
            <div className="flex flex-wrap items-center justify-between gap-3">
              <div className="text-sm text-slate-600 font-medium">
                Truy xuất trực tiếp phần tử gốc của <span className="font-bold text-amber-600 font-mono bg-amber-50 border border-amber-200 px-2 py-0.5 rounded">Max Heap O(1)</span> để lấy sinh viên có điểm GPA cao nhất toàn trường.
              </div>
              <Button 
                type="primary" 
                size="middle" 
                loading={loading}
                onClick={executeFastQuery}
                className="bg-amber-500 hover:bg-amber-400 text-white font-semibold shadow-sm ml-auto h-9 flex items-center gap-1.5"
                icon={<Zap className="w-4 h-4" />}
              >
                Thực Thi Thuật Toán Tối Ưu
              </Button>
            </div>
          )}

          {selectedTask === 'rq1' && (
            <div className="flex flex-col gap-2">
              <div className="flex items-center gap-2">
                <span className="text-[11px] text-indigo-600 bg-indigo-50 border border-indigo-200 px-2 py-0.5 rounded font-mono font-bold">
                  Dải lớp: C01 → C25 (hoặc 23DTH01...)
                </span>
              </div>
              <div className="flex flex-wrap items-center gap-2">
                <Input 
                  value={inputVal} 
                  onChange={(e) => setInputVal(e.target.value)} 
                  placeholder="Ví dụ: C01" 
                  className="w-44 font-mono font-bold text-indigo-600 h-9" 
                  size="middle" 
                  prefix={<Search className="w-4 h-4 text-slate-400" />}
                />
                <Button 
                  size="middle" 
                  icon={<Shuffle className="w-3.5 h-3.5" />}
                  onClick={pickRandomClass} 
                  className="text-xs font-semibold text-indigo-600 bg-indigo-50 border-indigo-200 hover:border-indigo-300 h-9"
                >
                  Ngẫu Nhiên
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal('C01')} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Đầu dải (C01)
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal('C13')} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Giữa dải (C13)
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal('C25')} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Cuối dải (C25)
                </Button>
                <Button 
                  size="middle" 
                  icon={<XCircle className="w-3.5 h-3.5" />}
                  onClick={() => setInputVal(nonExistClass)} 
                  className="text-xs font-medium text-rose-600 bg-rose-50 border-rose-200 hover:border-rose-300 h-9"
                >
                  Không tồn tại
                </Button>
                <Button 
                  type="primary" 
                  size="middle" 
                  loading={loading}
                  onClick={executeFastQuery}
                  className="bg-amber-500 hover:bg-amber-400 text-white font-semibold shadow-sm ml-auto h-9 flex items-center gap-1.5"
                  icon={<Zap className="w-4 h-4" />}
                >
                  Thực Thi Thuật Toán Tối Ưu
                </Button>
              </div>
            </div>
          )}

          {selectedTask === 'rq2' && (
            <div className="flex flex-wrap items-center justify-between gap-3">
              <div className="flex items-center gap-2">
                <label className="text-xs text-slate-600 font-semibold">Khoảng GPA:</label>
                <InputNumber 
                  min={0} 
                  max={10} 
                  step={0.1} 
                  value={gpaRange.min} 
                  onChange={(v) => setGpaRange(prev => ({ ...prev, min: v || 0 }))} 
                  size="middle"
                  className="w-24 h-9"
                />
                <span className="text-slate-500 text-xs font-medium">đến</span>
                <InputNumber 
                  min={0} 
                  max={10} 
                  step={0.1} 
                  value={gpaRange.max} 
                  onChange={(v) => setGpaRange(prev => ({ ...prev, max: v || 10 }))} 
                  size="middle"
                  className="w-24 h-9"
                />
              </div>
              <Button 
                type="primary" 
                size="middle" 
                loading={loading}
                onClick={executeFastQuery}
                className="bg-amber-500 hover:bg-amber-400 text-white font-semibold shadow-sm ml-auto h-9 flex items-center gap-1.5"
                icon={<Zap className="w-4 h-4" />}
              >
                Thực Thi Thuật Toán Tối Ưu
              </Button>
            </div>
          )}
        </div>
      </Card>

      {/* Result Presentation */}
      {result && (
        <div>
          {result.student && (
            <Card className="bg-white border border-emerald-200 shadow-sm rounded-xl">
              <div className="flex items-center justify-between mb-4">
                <span className="text-xs uppercase font-bold tracking-wider text-emerald-600 flex items-center gap-1.5">
                  <CheckCircle2 className="w-4 h-4" /> Đã Tìm Thấy Thông Tin Sinh Viên
                </span>
              </div>
              <div className="grid grid-cols-1 sm:grid-cols-4 gap-4 p-4 bg-slate-50 rounded-lg border border-slate-200">
                <div>
                  <span className="text-xs text-slate-500 block font-medium">MSSV</span>
                  <div className="text-lg font-mono font-bold text-blue-600 mt-1">{result.student.id}</div>
                </div>
                <div>
                  <span className="text-xs text-slate-500 block font-medium">Họ và Tên</span>
                  <div className="text-lg font-bold text-slate-800 mt-1">{result.student.name}</div>
                </div>
                <div>
                  <span className="text-xs text-slate-500 block font-medium">Mã Lớp</span>
                  <div className="text-lg font-bold text-slate-800 mt-1">{result.student.classId}</div>
                </div>
                <div>
                  <span className="text-xs text-slate-500 block font-medium">Điểm GPA</span>
                  <div className="text-lg font-bold text-emerald-600 mt-1">{result.student.gpa.toFixed(2)}</div>
                </div>
              </div>
            </Card>
          )}

          {result.students && (
            <Card className="bg-white border border-slate-200 shadow-sm rounded-xl">
              <div className="flex items-center justify-between mb-4">
                <h3 className="text-base font-bold text-slate-800 m-0">
                  Danh Sách Kết Quả ({result.students.length.toLocaleString()} Sinh viên)
                </h3>
              </div>
              <Table
                dataSource={result.students}
                rowKey="id"
                columns={[
                  { title: 'MSSV', dataIndex: 'id', key: 'id', render: (id: string) => <Tag color="blue" className="font-mono font-bold">{id}</Tag> },
                  { title: 'Họ và Tên', dataIndex: 'name', key: 'name', render: (name: string) => <span className="font-medium text-slate-800">{name}</span> },
                  { title: 'Lớp', dataIndex: 'classId', key: 'classId', render: (c: string) => <Tag color="geekblue">{c}</Tag> },
                  { title: 'Điểm GPA', dataIndex: 'gpa', key: 'gpa', render: (g: number) => <Tag color="green" className="font-bold">{g.toFixed(2)}</Tag> },
                ]}
                pagination={{ pageSize: 10, showSizeChanger: true }}
                bordered
                size="middle"
              />
            </Card>
          )}
        </div>
      )}
    </div>
  );
};
