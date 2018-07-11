CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(xannotation)

set(PHP_SOURCE /usr/include/php)
include_directories(${PHP_SOURCE})
include_directories(${PHP_SOURCE}/main)
include_directories(${PHP_SOURCE}/Zend)
include_directories(${PHP_SOURCE}/sapi)
include_directories(${PHP_SOURCE}/TSRM)
include_directories(~/xannotation)

add_executable(xannotation xannotation.c php_xannotation.h kernel/entries.c kernel/xan_class.c kernel/xan_class.h)

add_custom_target(makefile COMMAND make clean && /usr/local/php-7.1.15/bin/phpize && ./configure --with-php-config=/usr/local/php-7.1.15/bin/php-config && make && sudo make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})