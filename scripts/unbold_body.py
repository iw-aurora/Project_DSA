import sys

def remove_textbf(text):
    while '\\textbf{' in text:
        start = text.find('\\textbf{')
        depth = 0
        end = -1
        for i in range(start + 8, len(text)):
            if text[i] == '{':
                depth += 1
            elif text[i] == '}':
                if depth == 0:
                    end = i
                    break
                else:
                    depth -= 1
        if end != -1:
            inner = text[start + 8 : end]
            text = text[:start] + inner + text[end + 1:]
        else:
            break
    return text

with open('bao_cao/main.tex', 'r', encoding='utf-8') as f:
    lines = f.readlines()

new_lines = []
for idx, line in enumerate(lines):
    line_num = idx + 1
    
    # Preserve title page (lines 1 to 240)
    if line_num <= 240:
        new_lines.append(line)
        continue
    
    # Check if table header row
    stripped = line.strip()
    is_table_header = False
    if (stripped.startswith('\\textbf{STT}') or 
        stripped.startswith('\\textbf{Thành viên') or 
        stripped.startswith('\\textbf{Ký hiệu}') or 
        stripped.startswith('\\textbf{Tiêu chí}') or 
        stripped.startswith('\\textbf{Build structure}') or 
        stripped.startswith('\\textbf{Query workload}') or 
        stripped.startswith('\\textbf{Workload}')):
        is_table_header = True

    if is_table_header:
        new_lines.append(line)
    elif '\\textbf{' in line:
        new_lines.append(remove_textbf(line))
    else:
        new_lines.append(line)

with open('bao_cao/main.tex', 'w', encoding='utf-8') as f:
    f.writelines(new_lines)

print("Successfully unbolded all body text!")
