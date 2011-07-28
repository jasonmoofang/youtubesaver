project (YoutubeSaver)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

set (YoutubeSaver_SRCS
  YoutubeSaver.cpp
  MainWindow.cpp
)

kde4_add_executable(YoutubeSaver ${YoutubeSaver_SRCS})
target_link_libraries(YoutubeSaver ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBRARY} )