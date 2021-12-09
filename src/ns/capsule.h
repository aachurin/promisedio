
// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#define NS_API ns_api_4aaf83cd3102ad19b7f080e074b1b185

#define NS_API_CAPSULE {\
  [0] = Ns_GetAddrInfo,\
  [1] = Ns_GetNameInfo,\
  [2] = TcpStream_GetSockName,\
  [3] = TcpStream_GetPeerName,\
  [4] = TcpStream_SetTcpNoDelay,\
  [5] = TcpStream_SetTcpKeepAlive,\
  [6] = TcpStream_Connect,\
  [7] = Stream_Close,\
  [8] = Stream_Shutdown,\
  [9] = Stream_Write,\
  [10] = Stream_Read,\
  [11] = Stream_ReadExactly,\
  [12] = Stream_ReadUntil,\
}

