# Câu hỏi bảo vệ – Đặng Đoàn Minh Khang (RQ2)

Phạm vi: truy vấn sinh viên theo khoảng GPA bằng `SortedGpaFilter`, hai tìm kiếm nhị phân biên và tầng API bridge.

## Câu hỏi trọng tâm

1. **RQ2 giải quyết bài toán gì và vì sao không duyệt tuyến tính?**

   RQ2 tìm tất cả sinh viên có GPA thuộc đoạn đóng `[minGpa, maxGpa]`. Duyệt tuyến tính phải kiểm tra toàn bộ `N` sinh viên, tức `O(N)` cho mỗi truy vấn. Khi cần nhiều truy vấn trên dữ liệu ít thay đổi, tạo chỉ mục GPA một lần rồi tìm hai biên sẽ phù hợp hơn.

2. **Cấu trúc `SortedGpaFilter` lưu gì? Có sao chép toàn bộ sinh viên không?**

   Nó lưu `vector<const Student*> sortedStudents`: các con trỏ tới phần tử gốc trong `vector<Student>`, rồi sắp xếp các con trỏ theo GPA. Vì vậy không sao chép đối tượng `Student`; chi phí chỉ mục thêm là `O(N)` con trỏ.

3. **Pha xây dựng và pha truy vấn có độ phức tạp bao nhiêu?**

   Tạo con trỏ mất `O(N)`, sắp xếp mất `O(N log N)`. Mỗi truy vấn tìm hai biên mất `O(log N)`. Nếu chỉ trả về view thì kết quả là `O(log N)`; nếu phải duyệt/serialize `K` sinh viên thì tổng chi phí là `O(log N + K)`.

4. **Lower bound trong mã nguồn được cài đặt thế nào?**

   Vòng lặp đầu tìm chỉ số đầu tiên có `gpa >= minGpa`. Nếu `sortedStudents[mid]->gpa < minGpa` thì loại nửa trái bằng `left = mid + 1`; ngược lại giữ `mid` làm ứng viên bằng `right = mid`.

5. **Upper bound trong mã nguồn khác lower bound ở đâu?**

   Vòng lặp thứ hai tìm chỉ số đầu tiên có `gpa > maxGpa`. Nếu `gpa <= maxGpa` thì dịch `left = mid + 1`; ngược lại `right = mid`. Nhờ vậy các sinh viên có GPA đúng bằng `maxGpa` vẫn được giữ lại.

6. **Vì sao khoảng trả về là `[startIndex, endIndex)` thay vì hai chỉ số đóng?**

   Nửa khoảng có kích thước trực tiếp là `endIndex - startIndex`, biểu diễn rỗng tự nhiên khi hai biên bằng nhau và tránh lỗi cộng/trừ một. Mọi phần tử ở `startIndex` đến trước `endIndex` đều nằm trong đoạn GPA yêu cầu.

7. **Nếu nhiều sinh viên có cùng GPA đúng ở hai biên thì kết quả có đủ không?**

   Có. Lower bound chọn phần tử đầu tiên có GPA không nhỏ hơn `minGpa`; upper bound chọn vị trí ngay sau phần tử cuối cùng có GPA không lớn hơn `maxGpa`. Toàn bộ bản ghi bằng hai biên đều thuộc view.

8. **Tại sao hàm so sánh còn dùng `id` khi GPA bằng nhau?**

   `id` là tie-breaker để thứ tự kết quả xác định và ổn định theo cùng một dữ liệu. Nó không làm thay đổi tính đúng đắn của truy vấn GPA, nhưng giúp demo và kiểm thử tái lập được.

9. **Tại sao không dùng Max-Heap hoặc Hash Table cho RQ2?**

   Max-Heap chỉ đảm bảo cực trị ở đỉnh, không cho biết nhanh toàn bộ phần tử trong một đoạn GPA. Hash table mạnh ở truy vấn khóa chính xác, nhưng không duy trì thứ tự GPA. Mảng sắp xếp cho phép xác định liên tiếp hai biên bằng tìm kiếm nhị phân.

10. **Đánh đổi của giải pháp này là gì? Khi nào nó không còn phù hợp?**

   Đánh đổi là chi phí build `O(N log N)`, thêm `O(N)` con trỏ và cần build lại sau khi GPA/danh sách thay đổi. Nó phù hợp với workload đọc nhiều, cập nhật ít. Nếu chèn/xóa/cập nhật GPA diễn ra thường xuyên, nên cân nhắc balanced BST hoặc cơ chế chỉ mục động.

11. **Có rủi ro gì khi chỉ mục lưu con trỏ tới `vector<Student>`?**

   Nếu `vector<Student>` bị tái cấp phát hoặc phần tử bị xóa/sắp xếp làm địa chỉ thay đổi, các con trỏ trong chỉ mục có thể không còn hợp lệ. Sau mọi thay đổi làm mất ổn định địa chỉ hoặc GPA, cần xây dựng lại `SortedGpaFilter`; không được dùng view cũ.

12. **API bridge trả kết quả RQ2 cho web như thế nào?**

   Lệnh `gpa_range min max` gọi phiên bản baseline và phiên bản tối ưu, trả về số kết quả, bảng benchmark và preview tối đa 100 sinh viên. Bridge duyệt `optRes.at(i)` trong view để tạo JSON, nên thời gian hiển thị tỉ lệ với số bản ghi thực sự gửi về, không phải toàn bộ kết quả.

## Câu hỏi tình huống

13. **Nếu `minGpa > maxGpa` thì sao?**

   Về nghiệp vụ phải kiểm tra và từ chối/chuẩn hóa đầu vào trước khi truy vấn. Với cấu trúc view hiện tại, `size()` đã phòng thủ bằng cách trả `0` khi `endIndex < startIndex`; tuy nhiên API nên trả lỗi rõ ràng để người dùng không nhầm đây là một truy vấn hợp lệ không có kết quả.

14. **Nếu tất cả GPA đều nhỏ hơn `minGpa`, hoặc lớn hơn `maxGpa`, thuật toán có an toàn không?**

   Có. Hai tìm kiếm nhị phân trả về các biên trong đoạn từ `0` đến `N`; view rỗng có `startIndex == endIndex`. Không truy cập ngoài mảng.

15. **Nếu người dùng muốn danh sách kết quả được sắp xếp theo tên thay vì GPA thì sao?**

   Chỉ mục hiện tại bảo đảm thứ tự GPA rồi MSSV. Có thể sắp xếp riêng `K` kết quả theo tên với `O(K log K)`, hoặc duy trì thêm chỉ mục khác nếu nhu cầu này thường xuyên. Không nên phá thứ tự chỉ mục GPA vì sẽ mất khả năng binary search.

16. **Vì sao báo cáo nói `O(log N)` nhưng web vẫn có thể chậm khi khoảng rất rộng?**

   `O(log N)` chỉ là chi phí xác định hai biên và tạo view. Khi cần gửi/hiển thị nhiều kết quả, phải đọc và chuyển đổi từng bản ghi, nên có thêm `O(K)`. Bridge giới hạn preview 100 bản ghi để tránh phản hồi quá lớn; giao diện nên có phân trang nếu cần xem thêm.
