%define __libtoolize :
%define __spec_install_post /usr/lib/rpm/brp-compress
Name:             ots
Summary:          A text summarizer
Version:          0.5.0
Release:          1
License:          GPL
Group:            System Environment/Libraries
Source:           ots-%{version}.tar.gz
BuildRoot:        %{_tmppath}/%{name}-%{version}-root
BuildRequires:    pkgconfig >= 0.8
Requires:         glib2
BuildRequires:    glib2-devel libxml2-devel popt-devel

%description
The open text summarizer is an open source tool for summarizing texts.
The program reads a text and decides which sentences are important and
which are not.
The program can either print the summarized text in text format or in
HTML form where the important sentences are highlighted in red.
 
The program is multi lingual and work with UTF-8 code; 
 
The ots command line tool is an example and a debug tool for the libary.
You can bind to the library from your program.



%package devel
Summary:          Libraries and include files for developing with libots.
Group:            Development/Libraries
Requires:         %{name} = %{version}
Requires:         pkgconfig >= 0.8
Requires:         glib2
Requires:         glib2-devel libxml2-devel popt-devel


%description devel
This package provides the necessary development libraries and include
files to allow you to develop with libots.

%prep
%setup -q -n ots-%{version}

%build
%configure --disable-gtk-doc
sed -i 's/include $(top_srcdir)\/gtk-doc.make//g' doc/Makefile
make

%install
rm -rf $RPM_BUILD_ROOT

%makeinstall
# Clean out files that should not be part of the rpm.
# This is the recommended way of dealing with it for RH8
rm -f $RPM_BUILD_ROOT%{_libdir}/*.a
rm -f $RPM_BUILD_ROOT%{_libdir}/*.la

%clean
rm -rf $RPM_BUILD_ROOT

%files
%attr(0755,root,root)%{_bindir}/ots
%defattr(0644, root, root, 0755)
%doc AUTHORS COPYING ChangeLog NEWS README TODO
%{_libdir}/libots*
%{_datadir}/ots

%files devel
%defattr(0644, root, root, 0755)
%{_includedir}/libots-1/ots/*.h
%{_libdir}/pkgconfig/libots-1.pc
#%{_datadir}/doc/libots/html/*

%changelog
* Fri Mar 08 2013 Pavel Denisov <pavel.a.denisov@gmail.com>
- fix spec

* Thu Jun 05 2003 Rui Miguel Silva Seabra <rms@1407.org>
- fix spec
- disable gtk-doc (it's not building in RH 9,
  maybe it's broken for some reason)

* Fri May 02 2003 Rui Miguel Silva Seabra <rms@1407.org>
- define a longer description from the README file
- explicitly set file permissions

* Wed Apr 30 2003 Dom Lachowicz <cinamod@hotmail.com>
- created this thing
