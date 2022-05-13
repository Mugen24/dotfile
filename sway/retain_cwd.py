import socket
import os
import sys
import json
from pprint import pprint
from time import sleep

def recvall(sock, payload_len):
    BUFF_SIZE = 4096
    data = b''
    read_payload_len = False
    while True:
        if not read_payload_len:
            part = sock.recv(payload_len)
            read_payload_len = True


        part = sock.recv(BUFF_SIZE)
        data += part
        if len(part) < BUFF_SIZE:
            break
    return repr(data)

#convert response to json
def cleanup(sway_data):
    for index, char in enumerate(sway_data):
        if char == '[' or char == '{':
            delim = index
            break
    data = sway_data[delim: -1]
    try:
        _json = json.loads(data)
        return _json
    except Exception as e:
        #attemp to fix invalid \ when focused qtbrowser
        try:
            data = json.loads(data.translate({ord('\\') : None}))
            return data
        except Exception as e:
            print(e)
            with open("erro.log", "w") as fp:
                fp.write(data)
            exit()
            return data

def call_sock(p_type, message):
    magic = bytes("i3-ipc", "utf-8")

    message = bytes(message, "utf-8")
    p_type = p_type
    p_type = p_type.to_bytes(4, byteorder = sys.byteorder)
    length = len(message).to_bytes(4, byteorder = sys.byteorder)

    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect(os.getenv("SWAYSOCK"))
    s.send(magic+length+p_type+message)

    data = recvall(s, 14)
    _json = cleanup(data)
    return _json,s


def _focused(root):
    if root["nodes"] == []:
        if root["focused"]:
            return root
        return False

    for child in root["nodes"]:
        temp = _focused(child)
        if temp:
            break
    return temp

def get_focused():
    response ,sock = call_sock(4, '["GET_TREE"]')
    node = _focused(response)
    assert node != False
    return node

node = get_focused()
pprint(node)
