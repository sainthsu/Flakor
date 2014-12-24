###启动
	+ create-》config-》start-》resume=》inputQueueCreate=>NativeWindowCreate=>windowFocusChanged
###退出
	+ Pause =》WindowFocusChanged =>NativeWindowDestroyed=》stop=》InputQueueDestroyed=>Destroy
###进入后台
	+ SaveInstanceState=》Pause =》WindowFocusChanged=》NativeWindowDestroyed=>Stop
###唤起
	+ Start=》Resume=> NativeWindowCreated =>WindowFocusChanged
