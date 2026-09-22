'use client';

import React, { useState, useEffect } from 'react';
import { Card, Table, Tag, Button, Input, Modal, Form, InputNumber, Popconfirm, App } from 'antd';
import { Search, Plus, Edit3, Trash2 } from 'lucide-react';
import axios from 'axios';
import { Student } from '../types';

interface CrudTabProps {
  onDatasetChange: () => void;
}

export const CrudTab: React.FC<CrudTabProps> = ({ onDatasetChange }) => {
  const { message } = App.useApp();
  const [students, setStudents] = useState<Student[]>([]);
  const [loading, setLoading] = useState(false);
  const [page, setPage] = useState(1);
  const [total, setTotal] = useState(0);
  const [pageSize, setPageSize] = useState(15);
  const [search, setSearch] = useState('');

  // Modal states
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [editingStudent, setEditingStudent] = useState<Student | null>(null);
  const [form] = Form.useForm();

  const loadData = async (currentPage = 1, currentLimit = 15, currentSearch = search) => {
    setLoading(true);
    try {
      const res = await axios.get('/api/students', {
        params: {
          page: currentPage,
          limit: currentLimit,
          search: currentSearch,
        },
      });
      if (res.data.status === 'success') {
        setStudents(res.data.data);
        setTotal(res.data.total);
        setPage(res.data.page);
        setPageSize(res.data.limit);
      }
    } catch (err: any) {
      message.error('Lỗi khi tải danh sách sinh viên');
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    loadData(1, pageSize, search);
  }, []);

  const handleSearch = () => {
    loadData(1, pageSize, search);
  };

  const handleOpenAdd = () => {
    setEditingStudent(null);
    form.resetFields();
    setIsModalOpen(true);
  };

  const handleOpenEdit = (record: Student) => {
    setEditingStudent(record);
    form.setFieldsValue({
      id: record.id,
      name: record.name,
      classId: record.classId,
      gpa: record.gpa,
    });
    setIsModalOpen(true);
  };

  const handleDelete = async (id: string) => {
    try {
      const res = await axios.delete(`/api/students/${id}`);
      if (res.data.status === 'success') {
        message.success(`Đã xóa sinh viên ${id} thành công!`);
        loadData(page, pageSize, search);
        onDatasetChange();
      } else {
        message.error(res.data.message);
      }
    } catch (err: any) {
      message.error(err.response?.data?.message || 'Lỗi khi xóa sinh viên');
    }
  };

  const handleFormSubmit = async (values: any) => {
    try {
      if (editingStudent) {
        // Update
        const res = await axios.put(`/api/students/${editingStudent.id}`, values);
        if (res.data.status === 'success') {
          message.success('Cập nhật thông tin sinh viên thành công!');
          setIsModalOpen(false);
          loadData(page, pageSize, search);
          onDatasetChange();
        } else {
          message.error(res.data.message);
        }
      } else {
        // Create
        const res = await axios.post('/api/students', values);
        if (res.data.status === 'success') {
          message.success('Thêm sinh viên mới thành công!');
          setIsModalOpen(false);
          loadData(page, pageSize, search);
          onDatasetChange();
        } else {
          message.error(res.data.message);
        }
      }
    } catch (err: any) {
      message.error(err.response?.data?.message || 'Lỗi khi lưu thông tin');
    }
  };

  const columns = [
    {
      title: 'MSSV',
      dataIndex: 'id',
      key: 'id',
      render: (id: string) => <span className="font-mono font-bold text-blue-600">{id}</span>,
    },
    {
      title: 'Họ và Tên',
      dataIndex: 'name',
      key: 'name',
      render: (name: string) => <span className="font-medium text-slate-800">{name}</span>,
    },
    {
      title: 'Mã Lớp',
      dataIndex: 'classId',
      key: 'classId',
      render: (cls: string) => <Tag color="geekblue" className="font-semibold">{cls}</Tag>,
    },
    {
      title: 'Điểm GPA',
      dataIndex: 'gpa',
      key: 'gpa',
      render: (gpa: number) => (
        <Tag color={gpa >= 8.0 ? 'green' : gpa >= 6.5 ? 'blue' : 'orange'} className="font-bold">
          {gpa ? gpa.toFixed(2) : '0.00'}
        </Tag>
      ),
    },
    {
      title: 'Thao tác',
      key: 'actions',
      width: 140,
      render: (_: any, record: Student) => (
        <div className="flex items-center gap-2">
          <Button 
            type="text" 
            size="small" 
            icon={<Edit3 className="w-4 h-4 text-blue-600" />} 
            onClick={() => handleOpenEdit(record)} 
          />
          <Popconfirm
            title="Xác nhận xóa sinh viên"
            description={`Bạn có chắc muốn xóa sinh viên ${record.id}?`}
            onConfirm={() => handleDelete(record.id)}
            okText="Xóa"
            cancelText="Hủy"
            okButtonProps={{ danger: true }}
          >
            <Button 
              type="text" 
              size="small" 
              icon={<Trash2 className="w-4 h-4 text-rose-500" />} 
            />
          </Popconfirm>
        </div>
      ),
    },
  ];

  return (
    <div className="flex flex-col gap-4">
      <Card className="bg-white border border-slate-200 shadow-sm rounded-xl">
        <div className="flex flex-col md:flex-row items-stretch md:items-center justify-between gap-4">
          <div className="flex items-center gap-3">
            <Input 
              value={search} 
              onChange={(e) => setSearch(e.target.value)} 
              onPressEnter={handleSearch}
              placeholder="Tìm theo MSSV, Họ tên hoặc Mã lớp..." 
              className="w-full md:w-80" 
              size="middle"
              prefix={<Search className="w-4 h-4 text-slate-400" />}
              allowClear
            />
            <Button size="middle" onClick={handleSearch} className="font-medium">
              Tìm Kiếm
            </Button>
          </div>

          <Button 
            type="primary" 
            size="middle" 
            icon={<Plus className="w-4 h-4" />} 
            onClick={handleOpenAdd}
            className="bg-emerald-600 hover:bg-emerald-500 font-semibold shadow-sm"
          >
            Thêm Sinh Viên Mới
          </Button>
        </div>
      </Card>

      <Card className="bg-white border border-slate-200 shadow-sm rounded-xl">
        <Table
          dataSource={students}
          columns={columns}
          rowKey="id"
          loading={loading}
          bordered
          size="middle"
          pagination={{
            current: page,
            pageSize: pageSize,
            total: total,
            showSizeChanger: true,
            pageSizeOptions: ['15', '30', '50', '100'],
            onChange: (p, ps) => loadData(p, ps, search),
            showTotal: (tot) => `Tổng cộng ${tot.toLocaleString()} sinh viên`,
          }}
        />
      </Card>

      {/* Add / Edit Modal */}
      <Modal
        title={<span className="text-slate-800 font-bold">{editingStudent ? 'Chỉnh Sửa Thông Tin Sinh Viên' : 'Thêm Sinh Viên Mới'}</span>}
        open={isModalOpen}
        onCancel={() => setIsModalOpen(false)}
        footer={null}
        destroyOnClose
      >
        <Form
          form={form}
          layout="vertical"
          onFinish={handleFormSubmit}
          className="mt-4"
        >
          <Form.Item
            name="id"
            label="Mã Số Sinh Viên (MSSV)"
            rules={[{ required: true, message: 'Vui lòng nhập MSSV' }]}
          >
            <Input disabled={!!editingStudent} placeholder="Ví dụ: 25150999" />
          </Form.Item>

          <Form.Item
            name="name"
            label="Họ và Tên"
            rules={[{ required: true, message: 'Vui lòng nhập họ và tên' }]}
          >
            <Input placeholder="Ví dụ: Nguyen Van A" />
          </Form.Item>

          <Form.Item
            name="classId"
            label="Mã Lớp"
            rules={[{ required: true, message: 'Vui lòng nhập mã lớp' }]}
          >
            <Input placeholder="Ví dụ: C01, 23DTH1..." />
          </Form.Item>

          <Form.Item
            name="gpa"
            label="Điểm GPA (0.0 - 10.0)"
            rules={[
              { required: true, message: 'Vui lòng nhập điểm GPA' },
              { type: 'number', min: 0, max: 10, message: 'GPA phải từ 0.0 đến 10.0' }
            ]}
          >
            <InputNumber min={0} max={10} step={0.01} className="w-full" placeholder="Ví dụ: 8.50" />
          </Form.Item>

          <div className="flex justify-end gap-3 mt-6">
            <Button onClick={() => setIsModalOpen(false)}>Hủy</Button>
            <Button type="primary" htmlType="submit" className="bg-blue-600 hover:bg-blue-500 font-semibold">
              Lưu Thông Tin
            </Button>
          </div>
        </Form>
      </Modal>
    </div>
  );
};

