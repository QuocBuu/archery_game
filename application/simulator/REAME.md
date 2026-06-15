# STM32L151 OLED Simulator — Hướng dẫn từng bước cho Claude trong VSCode

Tài liệu này là tập lệnh bạn paste cho Claude trong VSCode để thêm một simulator layer vào project firmware STM32L151. Mục tiêu: cùng source code, build được hai output — firmware ARM cho chip thật, và executable Linux mô phỏng OLED bằng SDL2.

## Tổng quan kiến trúc

Thêm một build flag `SIMULATOR`. Khi flag bật, các hàm `HAL_*` mà driver OLED gọi sẽ được redirect sang lớp shim viết bằng C. Lớp shim dùng SDL2 để vẽ cửa sổ hiển thị framebuffer SSD1306 128×64 và đọc bàn phím làm nút bấm. Source game không phải sửa.

## Ràng buộc output (đọc trước khi bắt đầu)

Cửa sổ SDL phải tuân thủ tuyệt đối:

- Chỉ một cửa sổ duy nhất, kích thước chính xác 512×256 (scale x4 của 128×64).
- Toàn bộ cửa sổ là framebuffer SSD1306, không có vùng nào khác.
- Không vẽ nút bấm, D-pad, A/B trên cửa sổ.
- Không HUD, không FPS counter, không debug overlay, không border, không thanh trạng thái.
- Pixel ON = trắng (#FFFFFF), pixel OFF = đen (#000000). Không glow, không màu xanh giả OLED.
- Nút bấm chạy ngầm qua keyboard. Phím chỉ cập nhật GPIO state nội bộ.
- ESC = thoát. Đóng cửa sổ = thoát.

## Cách dùng tài liệu này

Mỗi STEP là một prompt độc lập. Paste từng STEP cho Claude theo thứ tự. Đợi Claude hoàn thành và xác nhận trước khi sang STEP tiếp theo. Không skip step.

---

## STEP 0 — Brief + Output spec

```
Tôi có project firmware STM32L151 dùng STM32 HAL. Phần cứng đích: 
màn OLED SSD1306 128x64 nối qua [SPI HOẶC I2C — bạn điền], và [SỐ] 
nút bấm nối GPIO.

Mục tiêu: thêm simulator layer build trên Linux. Cùng source code, 
khi định nghĩa SIMULATOR=1 thì build executable Linux. Không thì 
build firmware ARM bình thường. KHÔNG sửa source game.

=== OUTPUT SPEC — RÀNG BUỘC CỨNG, KHÔNG ĐƯỢC THAY ĐỔI ===

Cửa sổ SDL phải tuân thủ TUYỆT ĐỐI các điều sau:

1. CHỈ MỘT cửa sổ duy nhất. Không tạo cửa sổ phụ, không popup, 
   không splash screen của simulator.
   
2. Nội dung cửa sổ là MỘT VÙNG DUY NHẤT: framebuffer SSD1306 128×64 
   pixel, scale x4 → kích thước cửa sổ chính xác 512×256.
   
3. KHÔNG vẽ thêm bất cứ thứ gì khác trong cửa sổ:
   - KHÔNG panel nút bấm (không hiển thị D-pad, A/B, hay bất kỳ 
     nút nào trên màn hình)
   - KHÔNG HUD, không FPS counter, không debug overlay
   - KHÔNG border, viền, label, tiêu đề trong cửa sổ
   - KHÔNG thanh trạng thái, không thanh menu
   - KHÔNG vùng padding màu khác — toàn bộ 512×256 là pixel OLED
   
4. Title bar của OS (windowed) có thể đặt tên đơn giản như "STM32L151 
   Sim" — đây là chrome của OS, không phải nội dung vẽ.
   
5. Pixel ON = trắng (#FFFFFF), pixel OFF = đen (#000000). 
   KHÔNG dùng màu xanh dương kiểu OLED giả lập, KHÔNG hiệu ứng glow, 
   KHÔNG bo góc giả OLED. Hiển thị thuần đen-trắng giống framebuffer 
   thật.

6. Nút bấm CHẠY NGẦM hoàn toàn qua keyboard. Phím bấm chỉ cập nhật 
   GPIO state nội bộ. KHÔNG có visualization nào của nút trên cửa sổ.

7. ESC = thoát. Đóng cửa sổ = thoát. Không có gì khác.

=== HẾT OUTPUT SPEC ===

Cấu trúc project hiện tại:
[paste `tree` output]

Driver OLED ở: [đường dẫn]
HAL function driver gọi: [liệt kê — ví dụ HAL_SPI_Transmit, 
HAL_GPIO_WritePin, HAL_Delay, HAL_GetTick]

Trước khi tôi cho STEP tiếp theo, xác nhận:
(a) Hiểu output spec đúng — đặc biệt là KHÔNG vẽ nút bấm
(b) Liệt kê chính xác HAL function nào cần stub
(c) Hỏi tôi: OLED dùng SPI hay I2C, và nếu SPI thì chân D/C là 
    chân nào của firmware
```

---

## STEP 1 — Cấu trúc thư mục

```
Tạo:

sim/
├── CMakeLists.txt
├── include/
│   ├── stm32l1xx_hal.h
│   ├── stm32l1xx_hal_spi.h    (hoặc _i2c.h)
│   ├── stm32l1xx_hal_gpio.h
│   └── sim_internal.h          # API nội bộ sim, KHÔNG expose ra game
├── src/
│   ├── hal_shim.c
│   ├── sim_oled.c              # framebuffer + render OLED
│   ├── sim_input.c             # keyboard → GPIO state (KHÔNG render)
│   └── sim_main.c              # main loop, chỉ vẽ OLED
└── README.md

Rules:
- shim header trong sim/include/ là duy nhất khi SIMULATOR=1
- Không include header HAL gốc của ST trong sim build
- sim_input.c KHÔNG được gọi bất kỳ hàm SDL_Render* nào. Nó chỉ 
  poll event và set state array. Việc render trong cửa sổ là độc 
  quyền của sim_oled.c.

Tạo file skeleton rỗng, show tree sau khi xong.
```

---

## STEP 2 — HAL shim

```
Viết sim/include/stm32l1xx_hal.h + header con + sim/src/hal_shim.c.

Type tối thiểu:
- HAL_StatusTypeDef (HAL_OK=0, HAL_ERROR, HAL_BUSY, HAL_TIMEOUT)
- GPIO_TypeDef (struct giả, opaque)
- GPIO_PinState (GPIO_PIN_RESET=0, GPIO_PIN_SET=1)
- SPI_HandleTypeDef hoặc I2C_HandleTypeDef tuỳ chọn ở STEP 0

Macro GPIOA/GPIOB/... = con trỏ tới struct giả tĩnh.

Implementation hal_shim.c:
- HAL_SPI_Transmit / HAL_I2C_Master_Transmit:
    forward từng byte vào sim_oled_write_byte(b, is_command).
    SPI: is_command lấy từ GPIO state của chân D/C (hỏi tôi).
    I2C: byte đầu sau địa chỉ là control byte (0x00=cmd, 0x40=data).
- HAL_GPIO_WritePin: lưu state vào array tĩnh nội bộ
- HAL_GPIO_ReadPin: gọi sim_input_read_pin(port, pin), trả ngược 
  state nút. Convention: active-low (nhấn = 0)
- HAL_Delay: SDL_Delay
- HAL_GetTick: SDL_GetTicks
- HAL_Init, HAL_RCC_*, MX_*_Init: stub trả HAL_OK, không làm gì

QUAN TRỌNG: hal_shim.c KHÔNG gọi SDL_Render*, KHÔNG tạo cửa sổ. 
Việc tạo cửa sổ chỉ xảy ra ở sim_main.c → sim_oled_init().
```

---

## STEP 3 — Model SSD1306 + render OLED

```
Viết sim/src/sim_oled.c.

State:
- framebuffer[1024] (128 col × 8 page)
- col, page hiện tại
- col_start, col_end, page_start, page_end
- addr_mode (0=horizontal, 1=vertical, 2=page)
- display_on, contrast, inverted (cho lệnh A6/A7)

API:
- void sim_oled_init(void)
    Tạo SDL window, SDL renderer, SDL texture.
    Window: SDL_CreateWindow("STM32L151 Sim", CENTERED, CENTERED, 
            512, 256, 0).
    KHÔNG resizable, KHÔNG fullscreen, KHÔNG borderless trick.
    Texture: 128×64, SDL_PIXELFORMAT_RGB24 hoặc tương đương.
    
- void sim_oled_write_byte(uint8_t b, bool is_command)
    Command set giải mã đủ:
      0xAE/0xAF, 0x20 (+1), 0x21 (+2), 0x22 (+2), 0xB0-0xB7,
      0x00-0x0F, 0x10-0x1F, 0x40-0x7F, 0x81 (+1), 0xA0/0xA1,
      0xC0/0xC8, 0xA6/0xA7, 0xA8 (+1), 0xD3 (+1), 0xD5 (+1),
      0xD9 (+1), 0xDA (+1), 0xDB (+1), 0x8D (+1)
    Dùng state machine cho lệnh có param.
    Data: ghi framebuffer tại (col, page), tăng theo addr_mode, 
    wrap đúng khi vượt col_end/page_end.

- void sim_oled_render(void)
    1. SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)  // nền đen
    2. SDL_RenderClear(renderer)
    3. Convert framebuffer 128×64 → texture pixel buffer:
         for page 0..7, col 0..127:
           byte = framebuffer[page*128 + col]
           for bit 0..7:
             pixel_on = (byte >> bit) & 1
             color = pixel_on ? 0xFFFFFF : 0x000000
             nếu inverted thì đảo
             nếu !display_on thì luôn đen
             texture[(page*8+bit)*128 + col] = color
    4. SDL_UpdateTexture
    5. SDL_RenderCopy(renderer, texture, NULL, NULL)  
       // NULL dst rect = stretch fill toàn cửa sổ → scale x4 tự động
    6. SDL_RenderPresent

- void sim_oled_cleanup(void)

RÀNG BUỘC TUYỆT ĐỐI:
- Hàm sim_oled_render là HÀM DUY NHẤT trong toàn bộ project gọi 
  SDL_RenderClear / SDL_RenderCopy / SDL_RenderPresent.
- KHÔNG vẽ rect, line, text gì khác ngoài texture OLED.
- KHÔNG vẽ overlay nút bấm. KHÔNG vẽ FPS. KHÔNG vẽ border.
- Toàn bộ 512×256 cửa sổ = stretched copy của texture 128×64.

Code đầy đủ, không TODO.
```

---

## STEP 4 — Input ngầm

```
Viết sim/src/sim_input.c.

API:
- void sim_input_init(void)
- void sim_input_poll(void)  
    Gọi SDL_PollEvent trong vòng lặp, xử lý:
      SDL_QUIT → set running = false
      SDL_KEYDOWN/KEYUP → cập nhật state array
      ESC → set running = false
    KHÔNG gọi SDL_Render* gì cả. KHÔNG vẽ gì.
- bool sim_input_get_running(void)
- int sim_input_read_pin(GPIO_TypeDef* port, uint16_t pin)
    Trả GPIO_PIN_RESET nếu nút đang nhấn, GPIO_PIN_SET nếu thả 
    (active-low).

Mapping keyboard → nút (bạn điền chính xác theo firmware tôi sẽ 
cung cấp, mẫu):
  UP    → SDL_SCANCODE_UP
  DOWN  → SDL_SCANCODE_DOWN
  LEFT  → SDL_SCANCODE_LEFT
  RIGHT → SDL_SCANCODE_RIGHT
  A     → SDL_SCANCODE_J
  B     → SDL_SCANCODE_K

Hỏi tôi mapping nút ↔ (GPIO port, pin) trong firmware trước khi viết.

RÀNG BUỘC: file này KHÔNG bao giờ vẽ gì lên cửa sổ. Chỉ poll event 
và update state.
```

---

## STEP 5 — Main loop

```
Viết sim/src/sim_main.c:

extern void setup(void);   // hoặc tên entry firmware tôi cung cấp
extern void loop(void);

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);
    sim_oled_init();       // tạo cửa sổ — DUY NHẤT chỗ này
    sim_input_init();
    
    setup();
    
    while (sim_input_get_running()) {
        sim_input_poll();   // ngầm — không vẽ
        loop();             // game tick, có thể gọi vào HAL_SPI_Transmit
                            // → sim_oled_write_byte → cập nhật framebuffer
        sim_oled_render();  // CHỈ chỗ này vẽ ra cửa sổ
        SDL_Delay(16);
    }
    
    sim_oled_cleanup();
    SDL_Quit();
    return 0;
}

Nếu firmware tôi không có pattern setup/loop, hỏi tôi entry point 
thật. KHÔNG tự đoán.
```

---

## STEP 6 — CMakeLists

```
Viết sim/CMakeLists.txt:
- cmake_minimum_required 3.16
- project(stm32l151_sim C)
- find_package(SDL2 REQUIRED)
- add_executable(game_sim ...)
- include_directories: sim/include/ ĐẶT TRƯỚC mọi path khác
- target_compile_definitions: SIMULATOR=1
- C11, -Wall -Wextra -g
- target_link_libraries: SDL2

Hỏi tôi danh sách file firmware game đưa vào build. KHÔNG include:
  startup_stm32l151.s
  system_stm32l1xx.c
  file CubeMX phụ thuộc register thật

Output: cmake -B build_sim sim/ && cmake --build build_sim
        → build_sim/game_sim
```

---

## STEP 7 — Verify output đúng spec

```
Build và chạy ./build_sim/game_sim. Verify NGHIÊM NGẶT:

CHECKLIST OUTPUT (mỗi mục PHẢI pass, nếu fail thì sửa):
[ ] Cửa sổ kích thước CHÍNH XÁC 512×256
[ ] CHỈ MỘT cửa sổ, không có cửa sổ phụ
[ ] Toàn bộ cửa sổ là framebuffer OLED, không có vùng nào khác màu
[ ] KHÔNG hiển thị nút bấm, D-pad, A/B trên cửa sổ
[ ] KHÔNG có FPS counter, HUD, debug text
[ ] Pixel đúng đen-trắng, không màu xanh giả OLED, không glow
[ ] Nhấn phím UP/DOWN/A/B → game phản ứng, nhưng KHÔNG có gì hiện 
    trên cửa sổ ngoài thay đổi nội dung game
[ ] ESC thoát ngay, đóng X thoát ngay

Nếu BẤT KỲ mục nào fail, sửa ngay không hỏi lại tôi.
Chụp screenshot báo cáo.
```

---

## Lưu ý quan trọng trước khi bắt đầu

**Driver OLED truy cập trực tiếp thanh ghi.** Nếu driver dùng `SPI1->DR = byte` thay vì `HAL_SPI_Transmit`, shim không bắt được. Kiểm tra trước STEP 2. Nếu có, thêm `#ifdef SIMULATOR` quanh các đoạn đó hoặc bắt buộc driver dùng HAL.

**File phải loại khỏi build sim.** Mọi file đụng register STM32 thật (`startup_*.s`, `system_stm32l1xx.c`, driver low-level CMSIS, file CubeMX phụ thuộc HAL đầy đủ) không đưa vào build sim. Chỉ giữ game logic + driver OLED.

**Cài SDL2 trên Ubuntu/Debian:**

```
sudo apt install libsdl2-dev
```

**Build và chạy:**

```
cmake -B build_sim sim/
cmake --build build_sim
./build_sim/game_sim
```

**Nếu Claude lệch output spec** (vẽ thêm nút, thêm border, đổi màu), paste lại đoạn `OUTPUT SPEC` ở STEP 0 và bảo "Re-read the OUTPUT SPEC and fix."

## Sau khi MVP chạy

Nâng cấp khả thi mà không phá ràng buộc output: thêm âm thanh qua SDL_mixer (không vẽ thêm gì), thêm record GIF của vùng OLED, thêm gamepad mapping. Mọi nâng cấp đều phải giữ nguyên rule "chỉ một cửa sổ 512×256, chỉ vẽ framebuffer OLED".