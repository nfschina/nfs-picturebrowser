%global debug_package %{nil}
Name:		nfs-picturebrowser
Version:	1.0.9	
Release:	41.1%{?dist}
Summary:        picture browser
	

Group:		Application
License:        GPL	
Source0:	%{name}-%{version}.tar.gz	
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:	qt5-qtbase-devel
BuildRequires:	qt5-qtwebengine-devel
BuildRequires:	qt5-qtwebview-devel
BuildRequires:	qt5-qtscript-devel
BuildRequires:	qt5-qtsvg-devel


%description
 picture browser in nfs-desktop The software is a opensource package 

%prep
%setup -q


%build
make


%install
#%make_install
mkdir -p %{buildroot}/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/data/
mkdir -p %{buildroot}/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/platformthemes/
mkdir -p %{buildroot}/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/translations/
mkdir -p %{buildroot}/usr/share/cdos-picturebrowser/application/lib/
mkdir -p %{buildroot}/usr/share/applications/
mkdir -p %{buildroot}/usr/share/pixmaps/
mkdir -p %{buildroot}/usr/share/menu/
mkdir -p %{buildroot}/usr/bin/
mkdir -p %{buildroot}/usr/share/qt5/translations/


install -m 0755 cdos-picturebrowser.desktop  %{buildroot}/usr/share/applications/
install -m 0755 cdos-picturebrowser.png  %{buildroot}/usr/share/pixmaps/
install -m 0755 version  %{buildroot}/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/data/
install -m 0755 cdos-picturebrowser  %{buildroot}/usr/bin/cdos-picturebrowser
install -m 0755 platformthemes/libqgtk2.so  %{buildroot}/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/platformthemes/
install -m 0755 cdos-picturebrowser_en.qm  %{buildroot}/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/translations/
install -m 0755 qt_zh_CN.qm  %{buildroot}/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/translations/
install -m 0755 qtbase_zh_CN.qm  %{buildroot}/usr/share/qt5/translations/

%clean
rm -fr $RPM_BUILD_ROOT





%files
/usr/share/applications/cdos-picturebrowser.desktop
/usr/share/pixmaps/cdos-picturebrowser.png
/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/data/version
/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/platformthemes/libqgtk2.so
/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/translations/cdos-picturebrowser_en.qm
/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/translations/qt_zh_CN.qm
/usr/bin/cdos-picturebrowser
/usr/share/qt5/translations/qtbase_zh_CN.qm


%changelog

