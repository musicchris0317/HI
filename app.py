import streamlit as st
import google.generativeai as genai

# ====================== 初始化 Gemini API ======================
try:
    # 從 Streamlit Secrets 讀取 API Key
    api_key = st.secrets["GEMINI_API_KEY"]
    genai.configure(api_key=api_key)
    model = genai.GenerativeModel('gemini-1.5-flash')
except Exception as e:
    st.error(f"金鑰設定錯誤: {e}")
    st.stop()

# ====================== 頁面標題 ======================
st.title("🔗 Gemini API 連線測試")
st.info("若能成功收到 AI 回覆，表示你的 GitHub Secrets 環境配置正確！")

# ====================== 使用者輸入 ======================
user_input = st.text_input(
    "輸入一段話測試連線（例如：你好）",
    placeholder="請輸入文字..."
)

if user_input:
    with st.spinner("AI 正在思考中..."):
        try:
            # 呼叫 Gemini API
            response = model.generate_content(user_input)
            
            # 顯示回應
            st.subheader("🤖 AI 的回應：")
            st.write(response.text)
            
            st.success("✅ 連線成功！")
            
        except Exception as e:
            st.error(f"❌ 連線失敗: {e}")
