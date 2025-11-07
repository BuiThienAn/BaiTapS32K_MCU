## Giới thiệu khái niệm cơ bản về **CISC (Complex Instruction Set Computer)** và **RISC (Reduced Instruction Set Computer)**

CISC là viết tắt của Complex Instruction Set Computer (Máy tính với tập lệnh phức tạp). Đây là một kiến trúc thiết kế bộ vi xử lý, trong đó mỗi lệnh có thể thực hiện nhiều tác vụ cấp thấp khác nhau (như truy cập bộ nhớ và tính toán) trong một lệnh duy nhất. Kiến trúc này giúp giảm số lượng lệnh cần thiết cho một tác vụ, làm cho chương trình ngắn gọn hơn.

**RISC** (viết tắt của reduced instructions set computer - máy tính với tập lệnh đơn giản hóa) là một phương pháp thiết kế các vi xử lý theo hướng đơn giản hóa tập lệnh, trong đó thời gian thực thi tất cả các lệnh đều như nhau.

## Trình bày **ưu điểm và nhược điểm** của từng loại kiến trúc

| **Kiến trúc** | **Ưu điểm** | **Nhược điểm** |
| --- | --- | --- |
| **CISC** | **Tiết kiệm bộ nhớ (RAM) :** Cần ít lệnh hơn cho một chương trình<br><br>**Giảm tải cho trình biên dịch:** Ánh xạ lệnh cấp cao dễ dàng hơn<br><br>**Tính tương thích ngược:** Phần mềm cũ chạy được trên CPU mới | **Thiết kế CPU phức tạp:** Cần logic phức tạp để giải mã nhiều lệnh khác nhau.<br><br>**Khó tối ưu Pipelining:** Thời gian thực thi lệnh không đồng đều gây tắc nghẽn<br><br>**Tiêu thụ nhiều điện năng:** Logic phức tạp tốn nhiều transistor và tỏa nhiều nhiệt |
| **RISC** | **Hiệu suất Pipelining vượt trội:** Lệnh đơn giản, đồng nhất giúp "dây chuyền" chạy tối đa<br><br>**Thiết kế CPU đơn giản:** Logic giải mã dễ dàng, nhanh gọn<br><br>**Tiết kiệm điện năng:** Logic đơn giản, tiêu thụ ít điện và ít tỏa nhiệt<br><br>**Tận dụng không gian chip** | **Kích thước mã nguồn lớn hơn:** Cần nhiều lệnh hơn cho một tác vụ, làm "nở" chương trình.<br><br>**Phụ thuộc nặng vào trình biên dịch:** Hiệu suất phụ thuộc hoàn toàn vào khả năng tối ưu của trình biên dịch |

## So sánh CISC và RISC theo các tiêu chí

| **Tiêu chí** | **CISC (Complex Instruction Set Computer)** | **RISC (Reduced Instruction Set Computer)** |
| --- | --- | --- |
| **1.3.1 Cấu trúc tập lệnh** | **Phức tạp:** Có số lượng lệnh rất lớn (hàng trăm lệnh).<br><br>**Độ dài thay đổi:** Các lệnh có kích thước không đồng nhất<br><br>**Mạnh mẽ:** Một lệnh có thể thực hiện nhiều thao tác phức tạp cùng lúc | **Đơn giản hóa:** Tập lệnh nhỏ, chỉ chứa các lệnh cơ bản và thường xuyên sử dụng nhất<br><br>**Độ dài cố định:** Tất cả các lệnh có cùng kích thước giúp giải mã nhanh<br><br>**Đơn giản:** Mỗi lệnh chỉ thực hiện một thao tác cơ bản. |
| **1.3.2 Tốc độ xử lý** | Tập trung vào **giảm số lượng lệnh** trên mỗi chương trình<br><br>Một lệnh phức tạp cần **nhiều chu kỳ xung nhịp** (multi-cycle) để hoàn thành<br><br>Khó tối ưu Pipelining do các lệnh có thời gian chạy rất khác nhau. | Tập trung vào **giảm số chu kỳ xung nhịp** trên mỗi lệnh.<br><br>Hầu hết các lệnh được thiết kế để hoàn thành chỉ trong **một chu kỳ xung nhịp**<br><br>Rất hiệu quả và được thiết kế tối ưu cho Pipelining, giúp tăng thông lượng xử lý |
| **1.3.3Kích thước chương trình** | **Nhỏ gọn** | **Lớn** |
| **1.3.4. Độ phức tạp phần cứng** | **Cao.** Cần bộ giải mã (decoder) và logic điều khiển rất phức tạp để "hiểu" và thực thi hàng trăm lệnh khác nhau. Tốn nhiều transistor và diện tích chip hơn cho logic điều khiển | **Thấp.** Phần cứng đơn giản, bộ giải mã nhanh gọn do lệnh có định dạng cố định. |
| **1.3.5. Ứng dụng thực tế** | Thống trị thị trường **máy tính cá nhân (PC), máy tính xách tay (Laptop) và máy chủ (Server)**. | Thống trị thị trường **thiết bị di động** (điện thoại, máy tính bảng), **hệ thống nhúng**, và **IoT**. |

## Nêu **quan điểm cá nhân**: Trong bối cảnh phát triển hệ thống nhúng hiện nay, kiến trúc nào phù hợp hơn? Vì sao?

Trong bối cảnh phát triển hệ thống nhúng (embedded systems) hiện nay, **kiến trúc RISC là lựa chọn phù hợp hơn và đang chiếm ưu thế vượt trội.**

**1\. Hiệu quả Năng lượng (Performance-per-Watt):** Phần lớn thiết bị nhúng (cảm biến, thiết bị đeo tay, IoT, bộ điều khiển trong ô tô) hoạt động bằng pin hoặc có nguồn điện rất hạn chế. Chúng cũng thường được đặt trong các không gian kín, không có quạt tản nhiệt. Do có thiết kế phần cứng đơn giản, CPU RISC (như ARM) tiêu thụ ít điện năng hơn đáng kể và tỏa ít nhiệt hơn so với CISC.

**2\. Chi phí:** Các sản phẩm nhúng thường được sản xuất hàng loạt (hàng triệu, thậm chí hàng tỷ thiết bị). Việc giảm chi phí sản xuất, dù chỉ là vài xu trên mỗi con chip, cũng tạo ra sự khác biệt khổng lồ.

Kết luận

Trong khi CISC (x86) vẫn có giá trị trong các ứng dụng nhúng hiệu năng cao cần tương thích phần mềm, thì RISC (đặc biệt là ARM và sự trỗi dậy của RISC-V) rõ ràng là kiến trúc phù hợp hơn cho đại đa số các hệ thống nhúng hiện nay.

Triết lý thiết kế đơn giản, hiệu quả năng lượng, chi phí thấp và khả năng tùy biến cao của RISC hoàn toàn phù hợp với các yêu cầu cốt lõi của ngành công nghiệp nhúng.