import random
import sys

def generate_database(num_records=500000, output_path="data/database.json"):
    last_names = ['Nguyen', 'Tran', 'Le', 'Pham', 'Hoang', 'Huynh', 'Phan', 'Vu', 'Vo', 'Dang', 'Bui', 'Do', 'Ho', 'Ngo', 'Duong', 'Ly']
    middle_names = ['Van', 'Thi', 'Hoang', 'Minh', 'Gia', 'Quoc', 'Thanh', 'Duc', 'Ngoc', 'Dinh', 'Xuan', 'My', 'Huu', 'Kim']
    first_names = ['An', 'Binh', 'Nam', 'Anh', 'Bao', 'Ngoc', 'Huy', 'Tam', 'Long', 'Quan', 'Phuong', 'Kiet', 'Khang', 'Tien', 'Linh', 'Chau', 'Trang', 'Dung', 'Khoa', 'Phat']

    print(f"Dang tao {num_records:,} ban ghi vao {output_path}...")

    with open(output_path, "w", encoding="utf-8") as f:
        f.write('{\n    "students": [\n')
        for i in range(1, num_records + 1):
            s_id = f"25{i:06d}"
            name = f"{random.choice(last_names)} {random.choice(middle_names)} {random.choice(first_names)}"
            class_id = f"C{(i % 25) + 1:02d}"
            gpa = round(random.uniform(4.0, 10.0), 2)

            comma = "," if i < num_records else ""
            f.write(f'        {{\n            "classId": "{class_id}",\n            "gpa": {gpa:.2f},\n            "id": "{s_id}",\n            "name": "{name}"\n        }}{comma}\n')
            if i % 100000 == 0:
                print(f"-> Da tao xong {i:,} sinh vien...")
        f.write('    ]\n}\n')

    print(f"[Thanh cong] Hoan tat tao {num_records:,} ban ghi sinh vien vao {output_path}!")

if __name__ == "__main__":
    generate_database(500000, "data/database.json")

