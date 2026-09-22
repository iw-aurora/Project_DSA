export interface Student {
  id: string;
  name: string;
  classId: string;
  gpa: number;
}

export interface Stats {
  totalStudents: number;
  minGpa: number;
  maxGpa: number;
}

export interface BenchmarkMetric {
  metric: string;
  baseline: string;
  final: string;
}

export interface BenchmarkResult {
  status: string;
  module: string;
  benchmark: BenchmarkMetric[];
  student?: Student;
  students?: Student[];
  [key: string]: any;
}

