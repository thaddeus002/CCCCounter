Name:           cccc
Version:        3.1.5
Release:        1
License:        GPL
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  make, gcc, glibc-devel, gcc-c++, bison
BuildRequires:  libstdc++, libstdc++-devel


Group:          Development/Languages/C and C++
Summary:        C and C++ Code Counter
Source:         %{name}-%{version}.tar.gz

%description
CCCC is a tool which analyzes C++ and Java files and generates a report
on various metrics of the code. Metrics supported include
lines of code, McCabe's complexity and metrics i
proposed by Chidamber&Kemerer and Henry&Kafura.

%prep
%setup -q

%build
make



%install
install -d ${RPM_BUILD_ROOT}%{_bindir}
install src/cccc ${RPM_BUILD_ROOT}%{_bindir}

%clean
rm -rf "$RPM_BUILD_ROOT"

%files
%defattr(-,root,root)
%{_bindir}/*

%changelog
* Wed Jul 05 2017 Thaddeus002 <thaddeus.dupont[AT]free.fr>
- fix path
* Wed Apr 24 2013 Jan Ruzicka <jan.ruzicka[AT]comtechmobile.com> 3.2.1-1
- Initial RPM setup copied and modified from opensuse.org
