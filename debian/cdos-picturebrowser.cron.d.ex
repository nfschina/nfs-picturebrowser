#
# Regular cron jobs for the cdos-picturebrowser package
#
0 4	* * *	root	[ -x /usr/bin/cdos-picturebrowser_maintenance ] && /usr/bin/cdos-picturebrowser_maintenance
