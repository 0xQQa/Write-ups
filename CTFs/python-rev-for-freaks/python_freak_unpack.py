from base64 import b64decode

if __name__ == '__main__':
    with open('python_freak.py', 'r') as f:
        data = f.read()

    layer_count = 0
    while 'b64decode' in data:
        layer_count += 1
        data = data.split('\'')[1]
        data = b64decode(data).decode()

    print(f'{layer_count=}')
    with open('python_freak_unpacked.py', 'wb') as f:
        f.write(str.encode(data))
