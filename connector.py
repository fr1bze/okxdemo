import asyncio
import datetime
import websockets
import json


async def parser(ws): 
    async for msg in ws:
        msg = json.loads(msg)
        ev = msg.get('event')
        data = msg.get('data')
        if ev:
            print(f"---- event {ev} = {msg.get('arg')} -----")
        elif data and len(data) > 0:
            symbol = data[0].get('instId')
            price = float(data[0].get('markPx', '0.0'))
            print(f"{datetime.datetime.now().isoformat()} {symbol} = {price}")


async def connect_and_subscribe(url, subs):
    async with websockets.connect(url, ping_interval=20, ping_timeout=60) as ws:
        print("Connected " + datetime.datetime.now().isoformat())
        await ws.send(json.dumps(subs))
        await parser(ws)
    print("Disconnected " + datetime.datetime.now().isoformat())


async def make_ws_connections():
    url = 'wss://ws.okx.com:8443/ws/v5/public'
    subs = {
        'op': 'subscribe',
        'args': [
            {'channel': 'mark-price', 'instId': 'BTC-USDT'},
            {'channel': 'mark-price', 'instId': 'ETH-USDT-SWAP'},
        ]
    }
    tasks = [connect_and_subscribe(url, subs) for _ in range(10)]
    await asyncio.gather(*tasks)


if __name__ == "__main__":
    print("Hello")
    asyncio.run(make_ws_connections())
