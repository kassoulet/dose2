ffmpeg -r 50 -i "dosedump/dose2-dump-000%05d.ppm.png" -i italo160.ogg -acodec libvorbis -aq 6 -f webm -vpre libvpx-1080p50_60 -b 4000 -qmin 3 -qmax 30 -pass 1 -threads 8 dose2_by_mfx-ss4x-1080p50-tmp.webm
ffmpeg -r 50 -i "dosedump/dose2-dump-000%05d.ppm.png" -i italo160.ogg -acodec libvorbis -aq 6 -f webm -vpre libvpx-1080p50_60 -b 4000 -qmin 3 -qmax 30 -pass 2 -threads 8 dose2_by_mfx-ss4x-1080p50.webm

ffmpeg -r 50 -i "dosedump20x/dose2-dump-000%05d.ppm.png" -i italo160.ogg -acodec libvorbis -aq 6 -f webm -vpre libvpx-1080p50_60 -qmin 3 -qmax 30 -b 4000 -pass 1 -threads 8 dose2_by_mfx-ss4x_ts20x-1080p50-tmp.webm
ffmpeg -r 50 -i "dosedump20x/dose2-dump-000%05d.ppm.png" -i italo160.ogg -acodec libvorbis -aq 6 -f webm -vpre libvpx-1080p50_60 -qmin 3 -qmax 30 -b 4000 -pass 2 -threads 8 dose2_by_mfx-ss4x_ts20x-1080p50.webm


