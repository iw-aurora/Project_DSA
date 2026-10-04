with open('web_dsa/src/App.tsx', 'r', encoding='utf-8') as f:
    content = f.read()

old_f_block = """          {selectedTask === 'mc1' && (
            <div className="flex flex-col gap-1.5">
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
                  className="w-64 font-mono font-bold text-blue-600" 
                  size="middle" 
                  prefix={<Search className="w-4 h-4 text-slate-400" />}
                />
                <Button 
                  size="middle" 
                  onClick={pickRandomId} 
                  className="text-xs font-semibold text-blue-600 bg-blue-50 border-blue-200"
                >
                  🎲 Lấy Ngẫu Nhiên
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(String(minId))} 
                  className="text-xs font-medium"
                >
                  Đầu dải
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(String(maxId))} 
                  className="text-xs font-medium"
                >
                  Cuối dải
                </Button>
              </div>
            </div>
          )}

          {selectedTask === 'mc2' && (
            <div className="text-sm text-slate-600 font-medium">
              Truy xuất trực tiếp phần tử gốc của <span className="font-bold text-amber-600 font-mono bg-amber-50 px-2 py-0.5 rounded border border-amber-200">Max Heap O(1)</span> để lấy sinh viên có điểm GPA cao nhất toàn trường.
            </div>
          )}

          {selectedTask === 'rq1' && (
            <Input 
              value={inputVal} 
              onChange={(e) => setInputVal(e.target.value)} 
              placeholder="Nhập mã lớp cần lọc... (VD: C01, 23DTH1)" 
              className="w-80" 
              size="middle" 
            />
          )}"""

new_f_block = """          {selectedTask === 'mc1' && (
            <div className="flex flex-col gap-1.5">
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
                  className="w-44 font-mono font-bold text-blue-600" 
                  size="middle" 
                  prefix={<Search className="w-4 h-4 text-slate-400" />}
                />
                <Button 
                  size="middle" 
                  onClick={pickRandomId} 
                  className="text-xs font-semibold text-blue-600 bg-blue-50 border-blue-200 hover:border-blue-300"
                >
                  🎲 Ngẫu Nhiên
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(String(minId))} 
                  className="text-xs font-medium text-slate-600"
                >
                  Đầu dải
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(String(midId))} 
                  className="text-xs font-medium text-slate-600"
                >
                  Giữa dải
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(String(maxId))} 
                  className="text-xs font-medium text-slate-600"
                >
                  Cuối dải
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(nonExistId)} 
                  className="text-xs font-medium text-rose-600 bg-rose-50 border-rose-200 hover:border-rose-300"
                >
                  ❌ Không tồn tại
                </Button>
              </div>
            </div>
          )}

          {selectedTask === 'mc2' && (
            <div className="text-sm text-slate-600 font-medium">
              Truy xuất trực tiếp phần tử gốc của <span className="font-bold text-amber-600 font-mono bg-amber-50 px-2 py-0.5 rounded border border-amber-200">Max Heap O(1)</span> để lấy sinh viên có điểm GPA cao nhất toàn trường.
            </div>
          )}

          {selectedTask === 'rq1' && (
            <div className="flex flex-col gap-1.5">
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
                  className="w-44 font-mono font-bold text-indigo-600" 
                  size="middle" 
                  prefix={<Search className="w-4 h-4 text-slate-400" />}
                />
                <Button 
                  size="middle" 
                  onClick={pickRandomClass} 
                  className="text-xs font-semibold text-indigo-600 bg-indigo-50 border-indigo-200 hover:border-indigo-300"
                >
                  🎲 Ngẫu Nhiên
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal('C01')} 
                  className="text-xs font-medium text-slate-600"
                >
                  Đầu dải (C01)
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal('C13')} 
                  className="text-xs font-medium text-slate-600"
                >
                  Giữa dải (C13)
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal('C25')} 
                  className="text-xs font-medium text-slate-600"
                >
                  Cuối dải (C25)
                </Button>
                <Button 
                  size="middle" 
                  onClick={() => setInputVal(nonExistClass)} 
                  className="text-xs font-medium text-rose-600 bg-rose-50 border-rose-200 hover:border-rose-300"
                >
                  ❌ Không tồn tại
                </Button>
              </div>
            </div>
          )}"""

if old_f_block in content:
    content = content.replace(old_f_block, new_f_block)
    with open('web_dsa/src/App.tsx', 'w', encoding='utf-8') as f:
        f.write(content)
    print("FinalSolutionTab updated successfully!")
else:
    print("Error: old_f_block not found in App.tsx")

