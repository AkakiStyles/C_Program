# IPC 操作範例專案

本專案示範了三種主要的 IPC (Inter-Process Communication) 操作方式：
- **Message Queue**：`client` 透過訊息佇列將請求送給 `server`
- **Shared Memory**：`server` 處理完請求後，將執行結果記錄到共享記憶體
- **Semaphore**：`server` 與 `display` 之間透過信號量同步，`display` 會定期更新顯示共享記憶體中的執行結果

## 執行需求
1. Linux or Mac環境
2. gcc

## 專案結構

```
.
├── source
│   ├── libipc.c      # 整合信號量函式供server與display使用
│   ├── libtool.c     # Server處理client請求需要的函式
│   ├── client.c      # 負責向 server 發送請求
│   ├── display.c     # 週期性顯示共享記憶體的執行結果
│   └── server.c      # 處理 client 請求，並透過共享記憶體儲存結果
└── include
    └── demoipc.h     # 對應的標頭檔
```

## 編譯方式

1. 進入 `source` 資料夾：
   ```bash
   cd source
   ```
2. 執行 Makefile 進行編譯：
   ```bash
   make all
   ```
3. 編譯完成後，會產生執行檔（如 `client`, `server`, `display`）。

## 使用方式

1. **以兩個終端視窗分別執行 `server` 與 `display`**  
   - `server` 負責接收並處理來自 `client` 的請求。  
   - `display` 會定期讀取共享記憶體資料，並利用信號量與 `server` 進行同步。

2. **在另一個視窗執行 `client`**  
   - `client` 會透過訊息佇列發送請求給 `server`。  
   - 輸入以下三項資料（依序輸入）：  
     - **ID**：用來識別是哪個 `client` 發送的請求  
     - **Msg**：需要加密或解密的電文  
     - **Request Type**：告知 `server` 進行加密或解密,`'E'` 表示Encrypt,`'D'` 表示Decrypt  

---

### 範例流程

1. 開啟第一個終端視窗，執行：
   ```bash
   ./server
   ```
2. 開啟第二個終端視窗，執行：
   ```bash
   ./display
   ```
3. 在第三個終端視窗，執行：
   ```bash
   ./client
   ```
   依照提示，分別輸入 `ID`, `Msg`, 與 `Request Type`。

`server` 收到後會將處理結果寫入共享記憶體；`display` 則會從共享記憶體中讀取結果並顯示，同時透過信號量與 `server` 做同步。

---

