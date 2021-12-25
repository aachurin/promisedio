
// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#define NS_API ns_api_624b9c10cfcc03f67cca5a354d4f4885

#define NS_API_CAPSULE {\
  [0] = Ns_GetAddrInfo,\
  [1] = Ns_GetNameInfo,\
  [2] = TcpStream_GetSockName,\
  [3] = TcpStream_GetPeerName,\
  [4] = TcpStream_SetTcpNoDelay,\
  [5] = TcpStream_SetTcpKeepAlive,\
  [6] = Stream_Close,\
  [7] = Stream_Shutdown,\
  [8] = Stream_Write,\
  [9] = Stream_Read,\
  [10] = Stream_ReadExactly,\
  [11] = Stream_ReadUntil,\
  [12] = Stream_OpenConnection,\
  [13] = Stream_OpenUnixConnection,\
  [14] = UnixStream_GetPeerName,\
  [15] = UnixStream_GetSockName,\
  [16] = Stream_ServerClose,\
  [17] = Stream_StartServer,\
  [18] = Stream_StartUnixServer,\
}

