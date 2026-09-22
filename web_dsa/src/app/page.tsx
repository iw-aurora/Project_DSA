'use client';

import { useState, useEffect } from 'react';
import { ConfigProvider, App, theme, Tabs } from 'antd';
import { BarChart3, Zap, Database } from 'lucide-react';
import axios from 'axios';
import { Stats } from '../types';
import {
  Header,
  CriteriaBar,
  RamModal,
  BenchmarkTab,
  FinalSolutionTab,
  CrudTab,
} from '../components';

function DashboardContent() {
  const { message } = App.useApp();
  const [activeTab, setActiveTab] = useState('mode1');
  const [stats, setStats] = useState<Stats>({ totalStudents: 500000, minGpa: 4.0, maxGpa: 10.0 });
  const [loadingStats, setLoadingStats] = useState(false);
  const [ramModalOpen, setRamModalOpen] = useState(false);
  const [loadingRamAction, setLoadingRamAction] = useState(false);

  // Fast In-Memory Generator / Bulk Add
  const handleRamGenerate = async (count: number, isAdd = false) => {
    if (!count || isNaN(count) || count <= 0) {
      message.warning('Vui lòng nhập số lượng sinh viên hợp lệ!');
      return;
    }
    setLoadingRamAction(true);
    try {
      const endpoint = isAdd ? '/api/dataset/bulk-add' : '/api/dataset/generate';
      const res = await axios.post(endpoint, { count });
      if (res.data.status === 'success') {
        setStats({
          totalStudents: res.data.totalStudents,
          minGpa: res.data.minGpa,
          maxGpa: res.data.maxGpa,
        });
        message.success(
          (isAdd ? 'Đã thêm thành công ' : 'Đã nạp thành công ') +
          Number(count).toLocaleString() +
          ' sinh viên vào RAM trong ' +
          Number(res.data.timeMs).toFixed(1) +
          ' ms!'
        );
        setRamModalOpen(false);
      }
    } catch (err: any) {
      message.error(err.response?.data?.message || err.message || 'Lỗi khi thao tác RAM');
    } finally {
      setLoadingRamAction(false);
    }
  };

  const handleReloadDb = async () => {
    setLoadingRamAction(true);
    try {
      const res = await axios.post('/api/dataset/reload');
      if (res.data.status === 'success') {
        setStats({
          totalStudents: res.data.totalStudents,
          minGpa: res.data.minGpa,
          maxGpa: res.data.maxGpa,
        });
        message.success('Đã nạp lại dữ liệu gốc từ database.json vào RAM thành công!');
        setRamModalOpen(false);
      }
    } catch (err: any) {
      message.error(err.response?.data?.message || err.message || 'Lỗi khi nạp lại');
    } finally {
      setLoadingRamAction(false);
    }
  };

  // Fetch stats from backend C++ Bridge
  const fetchStats = async () => {
    setLoadingStats(true);
    try {
      const res = await axios.get('/api/stats');
      if (res.data.status === 'success') {
        setStats(res.data);
      }
    } catch (err) {
      console.error(err);
    } finally {
      setLoadingStats(false);
    }
  };

  useEffect(() => {
    fetchStats();
  }, []);

  return (
    <div className="min-h-screen bg-slate-100 text-slate-900 flex flex-col selection:bg-blue-500 selection:text-white">
      {/* Top Header Navbar */}
      <Header
        stats={stats}
        loadingStats={loadingStats}
        loadingRamAction={loadingRamAction}
        onOpenRamModal={() => setRamModalOpen(true)}
        onReloadDb={handleReloadDb}
        onRefreshStats={fetchStats}
      />

      {/* Main App Container */}
      <main className="container mx-auto w-full px-4 sm:px-6 lg:px-8 py-6 flex-1 flex flex-col gap-5">
        {/* Requirements Legend Strip */}
        <CriteriaBar />

        {/* Mode Tabs */}
        <Tabs
          activeKey={activeTab}
          onChange={setActiveTab}
          type="card"
          size="large"
          className="custom-tabs bg-transparent"
          items={[
            {
              key: 'mode1',
              label: (
                <span className="flex items-center gap-2 px-2 py-1 font-semibold text-slate-700">
                  <BarChart3 className="w-4 h-4 text-blue-600" />
                  <span>Mode 1: Benchmark Suite (So Sánh)</span>
                </span>
              ),
              children: <BenchmarkTab totalStudents={stats.totalStudents} />,
            },
            {
              key: 'mode2',
              label: (
                <span className="flex items-center gap-2 px-2 py-1 font-semibold text-slate-700">
                  <Zap className="w-4 h-4 text-amber-500" />
                  <span>Mode 2: Final Solution (Tối Ưu)</span>
                </span>
              ),
              children: <FinalSolutionTab totalStudents={stats.totalStudents} />,
            },
            {
              key: 'mode3',
              label: (
                <span className="flex items-center gap-2 px-2 py-1 font-semibold text-slate-700">
                  <Database className="w-4 h-4 text-emerald-600" />
                  <span>Mode 3: Quản Lý Sinh Viên (CRUD)</span>
                </span>
              ),
              children: <CrudTab onDatasetChange={fetchStats} />,
            },
          ]}
        />
      </main>

      {/* Fast RAM Injector Modal */}
      <RamModal
        open={ramModalOpen}
        totalStudents={stats.totalStudents}
        loading={loadingRamAction}
        onClose={() => setRamModalOpen(false)}
        onGenerate={handleRamGenerate}
        onReload={handleReloadDb}
      />

      {/* Footer */}
      <footer className="border-t border-slate-200 py-4 text-center text-xs text-slate-500 bg-white mt-auto">
        Dự án Cấu Trúc Dữ Liệu & Giải Thuật (DASA230179) • Next.js + Ant Design + Tailwind CSS + C++20 Core
      </footer>
    </div>
  );
}

export default function HomePage() {
  return (
    <ConfigProvider
      button={{ autoInsertSpace: false }}
      theme={{
        algorithm: theme.defaultAlgorithm,
        token: {
          colorPrimary: '#2563eb',
          borderRadius: 8,
          colorBgBase: '#ffffff',
          colorBgContainer: '#ffffff',
          colorBorderSecondary: '#e2e8f0',
          fontFamily: "'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif",
        },
      }}
    >
      <App>
        <DashboardContent />
      </App>
    </ConfigProvider>
  );
}
