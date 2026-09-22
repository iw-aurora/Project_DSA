'use client';

import React from 'react';
import { Button } from 'antd';
import { Cpu, Sparkles, RotateCcw, RefreshCw } from 'lucide-react';
import { Stats } from '../types';

interface HeaderProps {
  stats: Stats;
  loadingStats: boolean;
  loadingRamAction: boolean;
  onOpenRamModal: () => void;
  onReloadDb: () => void;
  onRefreshStats: () => void;
}

export const Header: React.FC<HeaderProps> = ({
  stats,
  loadingStats,
  loadingRamAction,
  onOpenRamModal,
  onReloadDb,
  onRefreshStats,
}) => {
  return (
    <header className="border-b border-slate-200 bg-white sticky top-0 z-50 shadow-sm">
      <div className="container mx-auto px-4 sm:px-6 lg:px-8 py-3.5 flex flex-col md:flex-row items-center justify-between gap-4">
        <div className="flex items-center gap-3">
          <div className="w-10 h-10 rounded-xl bg-blue-600 flex items-center justify-center shadow-md shadow-blue-500/20">
            <Cpu className="w-6 h-6 text-white" />
          </div>
          <div>
            <h1 className="text-xl font-bold text-slate-800 m-0">
              DSA Performance Dashboard
            </h1>
            <p className="text-xs text-slate-500 m-0 flex items-center gap-2 font-medium">
              <span>C++ High Performance Engine</span>
              <span className="inline-block w-1.5 h-1.5 rounded-full bg-emerald-500"></span>
              <span className="text-emerald-600 font-semibold">{stats.totalStudents.toLocaleString()} Dataset Active</span>
            </p>
          </div>
        </div>

        {/* RAM Injector & Quick Stats */}
        <div className="flex flex-wrap items-center gap-3">
          {/* Nạp RAM & Reset Button Pair */}
          <div className="flex items-center gap-2">
            <Button
              type="primary"
              icon={<Sparkles className="w-4 h-4 text-amber-300" />}
              onClick={onOpenRamModal}
              className="bg-blue-600 hover:bg-blue-500 font-bold shadow-sm flex items-center gap-1.5 h-10 px-4 rounded-xl"
              size="middle"
            >
              Nạp RAM
            </Button>
            <Button
              icon={<RotateCcw className="w-4 h-4 text-slate-600" />}
              loading={loadingRamAction}
              onClick={onReloadDb}
              className="font-semibold text-slate-700 hover:text-slate-900 border-slate-300 hover:border-slate-400 flex items-center gap-1 h-10 px-3.5 rounded-xl"
              size="middle"
              title="Khôi phục dữ liệu gốc từ database.json"
            >
              Reset
            </Button>
          </div>

          {/* Quick Metrics Bar */}
          <div className="flex items-center gap-4 bg-slate-50 border border-slate-200 px-4 py-2 rounded-xl shadow-inner">
            <div className="text-right">
              <div className="text-[11px] font-semibold text-slate-500 uppercase tracking-wider">Tổng Sinh Viên</div>
              <div className="text-base font-bold text-blue-600">
                {stats.totalStudents.toLocaleString()} SV
              </div>
            </div>
            <div className="h-6 w-px bg-slate-200"></div>
            <div className="text-right">
              <div className="text-[11px] font-semibold text-slate-500 uppercase tracking-wider">Phổ Điểm GPA</div>
              <div className="text-base font-bold text-emerald-600">
                {stats.minGpa.toFixed(1)} - {stats.maxGpa.toFixed(1)}
              </div>
            </div>
            <Button 
              type="text" 
              size="small" 
              icon={<RefreshCw className={`w-3.5 h-3.5 text-slate-500 ${loadingStats ? 'animate-spin' : ''}`} />} 
              onClick={onRefreshStats} 
            />
          </div>
        </div>
      </div>
    </header>
  );
};

