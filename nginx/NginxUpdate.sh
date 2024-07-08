rm -rf ./app/dep/*
echo "成功删除app/中dep文件"
rm -rf ./app/link_obj/*
echo "成功删除app/中link_obj文件"
echo "现在开始make操作"
truncate -s 0 error.log
make
