# RosCanSat: ПО бортового компьютера
Проект пишется для raspberry pi 3 под операционной системой raspbian.

## Программное обеспечение состоит из нескольких проектов.
- camera    - приложение, производящее фото съемку.
- telemetry - приложение, которое собирает информацию с преложений БК и передает ее в другие приложения.
- lib       - основные библиотеки проекта и приложение для их тестирования

## Инструкция по настройке.
Для работы проекта необходимо установить:
- Raspicam - api для управления камерой. Ссылка: https://www.uco.es/investiga/grupos/ava/node/40. А так же включить опции -lpthread и -lrt т.к. библиотека использует linux semaphore и -lraspicam для подключения самой библиотеки.
- GDBM - легкая база данных. Обычно в linux уже предустановлена. Ссылка: http://www.gnu.org.ua/software/gdbm/. Не забудте добавить -lgdbm в опции сборки.
