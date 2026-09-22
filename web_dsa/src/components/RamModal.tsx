'use client';

import React, { useState } from 'react';
import { Modal, Input, Button, Spin } from 'antd';
import { Sparkles, RotateCcw } from 'lucide-react';

interface RamModalProps {
  open: boolean;
  totalStudents: number;
  loading: boolean;
  onClose: () => void;
  onGenerate: (count: number, isAdd?: boolean) => void;
  onReload: () => void;
}

export const RamModal: React.FC<RamModalProps> = ({
  open,
  totalStudents,
  loading,
  onClose,
  onGenerate,
  onReload,
}) => {
  const [customCountStr, setCustomCountStr] = useState<string>('500,000');

  const handleInputChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const digits = e.target.value.replace(/\D/g, '');
    if (!digits) {
      setCustomCountStr('');
    } else {
      setCustomCountStr(Number(digits).toLocaleString());
    }
  };

  const handleSubmit = () => {
    const val = parseInt(customCountStr.replace(/\D/g, ''), 10);
    onGenerate(val, true);
  };

  return (
    <Modal
      title={
        <div className="flex items-center gap-2 text-slate-800 font-bold text-base">
          <Sparkles className="w-5 h-5 text-amber-500" />
          <span>Nạp Thêm Dữ Liệu Vào RAM (In-Memory Bulk Injector)</span>
        </div>
      }
      open={open}
      onCancel={() => !loading && onClose()}
      footer={null}
      width={500}
      centered
    >
      <div className="flex flex-col gap-5 py-3">
        <div className="bg-slate-50 border border-slate-200 rounded-xl p-3 flex items-center justify-between">
          <span className="text-xs font-semibold text-slate-600">Đang có trong RAM:</span>
          <span className="text-sm font-mono font-bold text-blue-600">
            {totalStudents.toLocaleString()} sinh viên
          </span>
        </div>

        {/* Full-width Text Input */}
        <div className="flex flex-col gap-2">
          <label className="text-xs font-bold text-slate-700">Điền số lượng sinh viên cần nạp thêm:</label>
          <Input
            type="text"
            value={customCountStr}
            onChange={handleInputChange}
            placeholder="Ví dụ: 500,000 hoặc 100,000..."
            className="w-full font-bold text-base h-11"
            size="large"
            onPressEnter={handleSubmit}
          />
        </div>

        {/* Action Buttons: Nạp RAM & Reset side-by-side */}
        <div className="flex items-center gap-3 pt-2 border-t border-slate-100">
          <Button
            type="primary"
            icon={<Sparkles className="w-4 h-4 text-amber-300" />}
            loading={loading}
            onClick={handleSubmit}
            className="bg-blue-600 hover:bg-blue-500 font-bold flex-1 h-11 text-sm shadow-md shadow-blue-500/10"
          >
            Nạp Thêm Vào RAM
          </Button>
          <Button
            icon={<RotateCcw className="w-4 h-4 text-slate-600" />}
            loading={loading}
            onClick={onReload}
            className="font-bold text-slate-700 hover:text-slate-900 border-slate-300 hover:border-slate-400 h-11 px-5"
          >
            Reset
          </Button>
        </div>

        {loading && (
          <div className="py-2 text-center flex items-center justify-center gap-2 text-xs text-blue-600 font-semibold">
            <Spin size="small" />
            <span>Đang sinh và cộng dồn sinh viên vào RAM...</span>
          </div>
        )}
      </div>
    </Modal>
  );
};

