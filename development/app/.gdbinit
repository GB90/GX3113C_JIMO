tar jtag jtag://192.168.100.100:8888
set $cr18=0x7d
load




define osd_reset
	set $i = 0
	set *0xa4800090 &=~1
	while ($i < 150)
        set $i = $i + 1
    end
	set *0xa4800090 |= 1
end



