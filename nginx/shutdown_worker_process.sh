# 杀掉后台运行的worker进程
ps -aux | grep "worker process" | grep -v "grep" | awk '{print $2}' | xargs kill -9