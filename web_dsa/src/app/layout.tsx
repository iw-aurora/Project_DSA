import React from 'react';
import type { Metadata } from 'next';
import { AntdRegistry } from '@ant-design/nextjs-registry';
import './globals.css';

export const metadata: Metadata = {
  title: 'DSA Performance Dashboard - C++ High Performance Engine',
  description: 'DSA Benchmark Suite & Student Management with C++ Engine',
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="vi">
      <body className="min-h-screen bg-slate-100 text-slate-900 selection:bg-blue-500 selection:text-white">
        <AntdRegistry>{children}</AntdRegistry>
      </body>
    </html>
  );
}
