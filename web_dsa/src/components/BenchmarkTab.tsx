'use client';

import React, { useState } from 'react';
import { Card, Button, Input, InputNumber, Table, Tag, Select, Spin, App } from 'antd';
import { Sliders, Flame, BarChart3, CheckCircle2, Layers, Shuffle, XCircle } from 'lucide-react';
import axios from 'axios';
import { BenchmarkResult } from '../types';

const { Option } = Select;

interface BenchmarkTabProps {
  totalStudents?: number;
}

export const BenchmarkTab: React.FC<BenchmarkTabProps> = ({ totalStudents = 500000 }) => {
  const { message } = App.useApp();
  const [moduleKey, setModuleKey] = useState('mc1');
  const [loading, setLoading] = useState(false);
  const [benchmarkResult, setBenchmarkResult] = useState<BenchmarkResult | null>(null);

  // Form states
  const [minGpa, setMinGpa] = useState<number>(8.0);
  const [maxGpa, setMaxGpa] = useState<number>(9.5);
  const [classId, setClassId] = useState<string>('C01');
  const [studentId, setStudentId] = useState<string>('25150001');

  const minId = 25150001;
  const midId = 25150000 + Math.floor(totalStudents / 2);
  const maxId = 25150000 + totalStudents;
  const nonExistId = '99999999';

  const validClasses = ['C01', 'C05', 'C08', 'C13', 'C18', 'C22', 'C25', '23DTH01', '24DTH02', '25KTP01'];
  const nonExistClass = 'C999';

  const pickRandomId = () => {
    const rand = Math.floor(Math.random() * totalStudents) + minId;
    setStudentId(String(rand));
  };

  const pickRandomClass = () => {
    const rand = validClasses[Math.floor(Math.random() * validClasses.length)];
    setClassId(rand);
  };

  const runBenchmark = async () => {
    setLoading(true);
    setBenchmarkResult(null);
    try {
      let res;
      if (moduleKey === 'mc1') {
        res = await axios.post('/api/benchmark/student-id', { studentId });
      } else if (moduleKey === 'mc2') {
        res = await axios.post('/api/benchmark/max-gpa');
      } else if (moduleKey === 'rq1') {
        res = await axios.post('/api/benchmark/class-id', { classId });
      } else if (moduleKey === 'rq2') {
        res = await axios.post('/api/benchmark/gpa-range', { minGpa, maxGpa });
      }
      if (res?.data?.status === 'success') {
        setBenchmarkResult(res.data);
        message.success('Đã hoàn thành phân tích hiệu năng benchmark!');
      }
    } catch (err: any) {
      message.error(err.response?.data?.message || err.message || 'Lỗi khi chạy benchmark');
    } finally {
      setLoading(false);
    }
  };

  const columns = [
    {
      title: 'Chỉ số đánh giá thuật toán',
      dataIndex: 'metric',
      key: 'metric',
      render: (text: string) => <span className="font-semibold text-slate-800">{text}</span>,
    },
    {
      title: 'Baseline (Giải thuật gốc)',
      dataIndex: 'baseline',
      key: 'baseline',
      render: (text: string) => (
        <span className="font-mono text-rose-700 bg-rose-50 border border-rose-200 px-3 py-1 rounded-md text-xs font-bold">
          {text}
        </span>
      ),
    },
    {
      title: 'Final Solution (Giải thuật tối ưu)',
      dataIndex: 'final',
      key: 'final',
      render: (text: string) => (
        <span className="font-mono text-emerald-700 bg-emerald-50 border border-emerald-200 px-3 py-1 rounded-md text-xs font-bold">
          {text}
        </span>
      ),
    },
  ];

  const studentColumns = [
    { title: 'MSSV', dataIndex: 'id', key: 'id', render: (id: string) => <Tag color="blue" className="font-mono font-bold">{id}</Tag> },
    { title: 'Họ và Tên', dataIndex: 'name', key: 'name', render: (name: string) => <span className="font-medium text-slate-900">{name}</span> },
    { title: 'Mã Lớp', dataIndex: 'classId', key: 'classId', render: (cls: string) => <Tag color="geekblue">{cls}</Tag> },
    { 
      title: 'Điểm GPA', 
      dataIndex: 'gpa', 
      key: 'gpa', 
      render: (gpa: number) => (
        <Tag color={gpa >= 8.5 ? 'gold' : gpa >= 7.0 ? 'green' : 'default'} className="font-bold">
          {gpa ? gpa.toFixed(2) : '0.00'}
        </Tag>
      )
    },
  ];

  return (
    <div className="flex flex-col gap-6">
      {/* Module Selector Card */}
      <Card className="bg-white border border-slate-200 shadow-sm rounded-xl p-2">
        <div className="flex flex-col xl:flex-row items-start xl:items-center justify-between gap-4 pb-4 border-b border-slate-100">
          <div>
            <h2 className="text-base font-bold text-slate-800 flex items-center gap-2 m-0">
              <Sliders className="w-5 h-5 text-blue-600" />
              Thiết Lập Bài Toán & Tham Số Kiểm Thử
            </h2>
            <p className="text-xs text-slate-500 mt-1 m-0">
              So sánh hiệu năng chi tiết giữa giải thuật cơ sở (Baseline) và tối ưu (Final Solution) trên tập 500k phần tử.
            </p>
          </div>

          {/* Select Module + Benchmark Button Pair side-by-side with same height */}
          <div className="flex flex-wrap items-center gap-3 w-full xl:w-auto">
            <Select
              value={moduleKey}
              onChange={(val) => {
                setModuleKey(val);
                setBenchmarkResult(null);
              }}
              className="w-full sm:w-80 md:w-96"
              size="large"
            >
              <Option value="mc1">[BẮT BUỘC] MC1: Tra cứu theo MSSV (Hash Table O(1))</Option>
              <Option value="mc2">[BẮT BUỘC] MC2: Sinh viên GPA cao nhất (Max-Heap O(1))</Option>
              <Option value="rq1">[TỰ CHỌN] RQ1: Lọc theo Mã Lớp (Optimized Index)</Option>
              <Option value="rq2">[TỰ CHỌN] RQ2: Lọc khoảng GPA (Binary Search O(log N + K))</Option>
            </Select>

            <Button 
              type="primary" 
              size="large" 
              icon={<Flame className="w-4 h-4" />} 
              loading={loading}
              onClick={runBenchmark}
              className="bg-blue-600 hover:bg-blue-500 font-bold shadow-sm flex items-center gap-2 px-5 h-10 rounded-lg"
            >
              Chạy Benchmark Đối Sánh (1000 Workload)
            </Button>
          </div>
        </div>

        {/* Dynamic Parameter Inputs & Helpers */}
        <div className="mt-4 flex flex-col gap-2.5">
          {moduleKey === 'mc1' && (
            <>
              <div className="flex items-center gap-2">
                <label className="text-xs font-bold text-slate-700">Mã Sinh Viên (MSSV):</label>
                <span className="text-[11px] text-blue-600 bg-blue-50 border border-blue-200 px-2 py-0.5 rounded font-mono font-bold">
                  Dải hợp lệ: {minId} → {maxId.toLocaleString()}
                </span>
              </div>
              <div className="flex flex-wrap items-center gap-2">
                <Input 
                  value={studentId} 
                  onChange={(e) => setStudentId(e.target.value)} 
                  placeholder={`Ví dụ: ${minId}`} 
                  className="w-48 font-mono font-bold text-blue-600 h-9" 
                  size="middle" 
                />
                <Button 
                  size="middle" 
                  icon={<Shuffle className="w-3.5 h-3.5" />}
                  onClick={pickRandomId} 
                  className="text-xs font-semibold text-blue-600 hover:text-blue-700 bg-blue-50 border-blue-200 hover:border-blue-300 h-9"
                >
                  Ngẫu Nhiên
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setStudentId(String(minId))} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Đầu dải
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setStudentId(String(midId))} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Giữa dải
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setStudentId(String(maxId))} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Cuối dải
                </Button>
                <Button 
                  size="middle" 
                  icon={<XCircle className="w-3.5 h-3.5" />}
                  onClick={() => setStudentId(nonExistId)} 
                  className="text-xs font-medium text-rose-600 bg-rose-50 border-rose-200 hover:border-rose-300 h-9"
                >
                  Không tồn tại
                </Button>
              </div>
            </>
          )}

          {moduleKey === 'mc2' && (
            <div className="text-sm text-slate-600 font-medium py-1">
              Tra cứu sinh viên có điểm GPA cao nhất (Extreme / Priority) trên cấu trúc <span className="text-amber-600 font-mono font-bold bg-amber-50 border border-amber-200 px-2 py-0.5 rounded">Custom Max Heap O(1)</span>.
            </div>
          )}

          {moduleKey === 'rq1' && (
            <>
              <div className="flex items-center gap-2">
                <label className="text-xs font-bold text-slate-700">Mã Lớp Cần Lọc:</label>
                <span className="text-[11px] text-indigo-600 bg-indigo-50 border border-indigo-200 px-2 py-0.5 rounded font-mono font-bold">
                  Dải lớp: C01 → C25 (hoặc 23DTH01...)
                </span>
              </div>
              <div className="flex flex-wrap items-center gap-2">
                <Input 
                  value={classId} 
                  onChange={(e) => setClassId(e.target.value)} 
                  placeholder="Ví dụ: C01" 
                  className="w-48 font-mono font-bold text-indigo-600 h-9" 
                  size="middle" 
                />
                <Button 
                  size="middle" 
                  icon={<Shuffle className="w-3.5 h-3.5" />}
                  onClick={pickRandomClass} 
                  className="text-xs font-semibold text-indigo-600 hover:text-indigo-700 bg-indigo-50 border-indigo-200 hover:border-indigo-300 h-9"
                >
                  Ngẫu Nhiên
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setClassId('C01')} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Đầu dải (C01)
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setClassId('C13')} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Giữa dải (C13)
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setClassId('C25')} 
                  className="text-xs font-medium text-slate-600 h-9"
                >
                  Cuối dải (C25)
                </Button>
                <Button 
                  size="middle" 
                  icon={<XCircle className="w-3.5 h-3.5" />}
                  onClick={() => setClassId(nonExistClass)} 
                  className="text-xs font-medium text-rose-600 bg-rose-50 border-rose-200 hover:border-rose-300 h-9"
                >
                  Không tồn tại
                </Button>
              </div>
            </>
          )}

          {moduleKey === 'rq2' && (
            <div className="flex items-center gap-3">
              <label className="text-xs font-bold text-slate-700">Khoảng điểm GPA:</label>
              <InputNumber min={0} max={10} step={0.1} value={minGpa} onChange={(v) => setMinGpa(v || 0)} className="w-32 h-9" size="middle" />
              <span className="text-slate-400 text-xs font-semibold">đến</span>
              <InputNumber min={0} max={10} step={0.1} value={maxGpa} onChange={(v) => setMaxGpa(v || 10)} className="w-32 h-9" size="middle" />
            </div>
          )}
        </div>
      </Card>

      {/* Results View */}
      {loading && (
        <Card className="bg-white border border-slate-200 py-16 text-center rounded-xl">
          <Spin size="large" />
          <p className="mt-4 text-sm font-medium text-slate-600">Đang kích hoạt Core C++ và chạy 1,000 vòng lặp kiểm thử hiệu năng...</p>
        </Card>
      )}

      {benchmarkResult && !loading && (
        <div className="flex flex-col gap-6">
          {/* Benchmark Table */}
          <Card className="bg-white border border-slate-200 shadow-sm rounded-xl">
            <div className="flex items-center justify-between mb-4">
              <h3 className="text-base font-bold text-slate-800 flex items-center gap-2 m-0">
                <BarChart3 className="w-5 h-5 text-emerald-600" />
                Bảng Thống Kê & Đối Sánh Hiệu Năng Chi Tiết
              </h3>
              <Tag color="blue" className="font-semibold px-3 py-1 text-xs">
                {benchmarkResult.module}
              </Tag>
            </div>

            <Table 
              dataSource={benchmarkResult.benchmark} 
              columns={columns} 
              pagination={false} 
              rowKey="metric"
              bordered
              size="middle"
            />
          </Card>

          {/* Single Student / Array of Students Output */}
          {benchmarkResult.student && (
            <Card className="bg-blue-50/50 border border-blue-200 shadow-sm rounded-xl">
              <div className="flex items-center gap-2 mb-3">
                <CheckCircle2 className="w-5 h-5 text-emerald-600" />
                <h4 className="text-sm font-bold text-slate-800 m-0">Kết Quả Tìm Thấy Trực Tiếp</h4>
              </div>
              <div className="grid grid-cols-2 sm:grid-cols-4 gap-4 p-4 bg-white rounded-lg border border-slate-200">
                <div>
                  <span className="text-xs text-slate-500 block font-medium">MSSV:</span>
                  <span className="text-base font-mono font-bold text-blue-600">{benchmarkResult.student.id}</span>
                </div>
                <div>
                  <span className="text-xs text-slate-500 block font-medium">Họ và Tên:</span>
                  <span className="text-base font-bold text-slate-800">{benchmarkResult.student.name}</span>
                </div>
                <div>
                  <span className="text-xs text-slate-500 block font-medium">Lớp:</span>
                  <span className="text-base font-bold text-slate-800">{benchmarkResult.student.classId}</span>
                </div>
                <div>
                  <span className="text-xs text-slate-500 block font-medium">Điểm GPA:</span>
                  <span className="text-base font-bold text-emerald-600">{benchmarkResult.student.gpa.toFixed(2)}</span>
                </div>
              </div>
            </Card>
          )}

          {benchmarkResult.students && (
            <Card className="bg-white border border-slate-200 shadow-sm rounded-xl">
              <div className="flex items-center justify-between mb-4">
                <h3 className="text-base font-bold text-slate-800 flex items-center gap-2 m-0">
                  <Layers className="w-5 h-5 text-blue-600" />
                  Danh Sách Kết Quả Trả Về ({benchmarkResult.students.length.toLocaleString()} Sinh viên)
                </h3>
              </div>

              <Table
                dataSource={benchmarkResult.students}
                columns={studentColumns}
                rowKey="id"
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
