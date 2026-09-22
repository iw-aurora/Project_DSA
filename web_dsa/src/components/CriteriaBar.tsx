'use client';

import React from 'react';

export const CriteriaBar: React.FC = () => {
  return (
    <div className="bg-white border border-slate-200 rounded-xl p-3 shadow-sm flex flex-col md:flex-row items-start md:items-center justify-between gap-3">
      <div className="flex items-center gap-2">
        <span className="text-xs font-bold uppercase tracking-wider text-slate-700">Mã Yêu Cầu & Tiêu Chí:</span>
      </div>
      <div className="flex flex-wrap items-center gap-2 text-xs">
        <span className="inline-flex items-center gap-1.5 bg-blue-50 border border-blue-200 text-blue-700 px-2.5 py-1 rounded-lg font-semibold">
          <span className="w-2 h-2 rounded-full bg-blue-600"></span>
          <b className="font-mono">MC1 (Bắt buộc):</b> Tra cứu MSSV [Hash Table O(1)]
        </span>
        <span className="inline-flex items-center gap-1.5 bg-amber-50 border border-amber-200 text-amber-800 px-2.5 py-1 rounded-lg font-semibold">
          <span className="w-2 h-2 rounded-full bg-amber-500"></span>
          <b className="font-mono">MC2 (Bắt buộc):</b> SV Max GPA [Heap O(1) Peek]
        </span>
        <span className="inline-flex items-center gap-1.5 bg-purple-50 border border-purple-200 text-purple-700 px-2.5 py-1 rounded-lg font-semibold">
          <span className="w-2 h-2 rounded-full bg-purple-500"></span>
          <b className="font-mono">RQ1 (Tự chọn):</b> Lọc Theo Mã Lớp [Vector Index]
        </span>
        <span className="inline-flex items-center gap-1.5 bg-emerald-50 border border-emerald-200 text-emerald-700 px-2.5 py-1 rounded-lg font-semibold">
          <span className="w-2 h-2 rounded-full bg-emerald-500"></span>
          <b className="font-mono">RQ2 (Tự chọn):</b> Lọc Khoảng GPA [O(log N + K)]
        </span>
      </div>
    </div>
  );
};

