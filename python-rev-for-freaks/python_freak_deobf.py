from unicodedata import normalize

if __name__ == '__main__':
    with open('python_freak_unpacked.py', 'rb') as f:
        data = f.read().decode("UTF-8")

    data = normalize('NFKD', data)
    data = data.replace('(lambda', '\n(lambda')
    print(data)
