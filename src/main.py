import serial
import time
from fastapi import FastAPI, Response, WebSocket
from fastapi.middleware.cors import CORSMiddleware

origins = ["*"]
app = FastAPI()
app.add_middleware(CORSMiddleware, allow_origins=origins, allow_credentials=True, allow_methods=["*"], allow_headers=["*"],)

ser = serial.Serial('/dev/ttyACM0', 9600)

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    try:
        while True:
            data = await websocket.receive_text()
            print(data)
            if data in ["F"]:
                ser.write(b'0')
            elif data in ["L"]:
                ser.write(b'9')
    except Exception as e:
        print(e)

##ser.reset_input_buffer()

