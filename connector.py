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
        print(f"Connected {datetime.datetime.now().isoformat()} to {subs['instId']}")
        await ws.send(json.dumps({'op': 'subscribe', 'args': [subs]}))
        await parser(ws)
    print(f"Disconnected {datetime.datetime.now().isoformat()} from {subs['instId']}")


async def make_ws_connections():
    url = 'wss://ws.okx.com:8443/ws/v5/public'
    subscriptions = [
        {'channel': 'mark-price', 'instId': 'BTC-USDT'},
        {'channel': 'mark-price', 'instId': 'ETH-USDT-SWAP'},
    ]
    tasks = []
    for subs in subscriptions:
        for _ in range(10):
            tasks.append(connect_and_subscribe(url, subs))
            await asyncio.sleep(0.01)
    await asyncio.gather(*tasks)


if __name__ == "__main__":
    print("Hello")
    asyncio.run(make_ws_connections())
