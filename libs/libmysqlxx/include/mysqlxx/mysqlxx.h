#ifndef MYSQLXX_H
#define MYSQLXX_H

#include <mysqlxx/Connection.h>
#include <mysqlxx/Transaction.h>
#include <mysqlxx/Manip.h>
#include <mysqlxx/Pool.h>

/** mysqlxx - чрезвычайно простая библиотека для замены библиотеки mysql++.
  *
  * По некоторой причине, в Яндекс.Метрике изначально использовалась библиотека mysql++.
  * Она обладает следующими недостатками:
  * 1. Чрезмерная навороченность: большая часть не используется.
  * 2. Низкая производительность (при использовании в Яндекс.Метрике).
  *
  * Низкая производительность обусловлена следующими причинами:
  *
  * 1. Лишние копирования: класс mysqlpp::Row работает как std::vector<std::string>.
  *    В него копируется содержимое MYSQL_ROW.
  *    MYSQL_ROW представляет собой char**, выделяемый одним куском,
  *     в котором подряд лежат значения в виде не-0-terminated строк.
  *
  * 2. Медленные методы конвертации значений в числа.
  *    В mysql++ для этого используется std::stringstream.
  *    Это работает медленнее функций POSIX (strtoul и т. п.).
  *    В свою очередь, это работает медленнее простых функций,
  *     написанных вручную, в которых не учитывается локаль и
  *     не используемые в MySQL представления чисел.
  *
  * 3. Медленные методы квотирования и эскейпинга значений.
  *    В mysql++ используется mysql_real_escape_string, который корректно
  *     эскейпит строки даже при использовании кодировок, не основанных на ASCII.
  *    Но при использовании кодировок, основанных на ASCII, например UTF-8,
  *     (в общем, кодировок, в которых escape-символы передаются как в ASCII,
  *      и данные байты не могут встречаться в представлении каких-либо других символов)
  *     эта функция избыточна.
  *
  * 4. Много мусора.
  *
  * Низкая производительность проявляется в виде следующих эффектов:
  * 1. При последовательном чтении таблицы из MySQL, узким местом является процессор клиента,
  *     на котором выполняется код с использованием mysql++, а MySQL сервер не нагружен.
  *    При использовании MySQL C API этого не возникает.
  * 2. Скорость последовательного чтения из MySQL не превышает 30MB/s.
  *
  * ВНИМАНИЕ!
  *
  * mysqlxx реализована как чрезмерно простая обёртка над MySQL C API,
  *  реализующая небольшое подмножество интерфейса mysql++, которое мы используем.
  * При этом, ради простоты и производительности приносится в жертву
  *  кроссплатформенность, меняется семантика некоторых функций,
  *  многие функции работают при большом наборе допущений.
  * Допущения зачастую специфичны именно для Яндекс.Метрики и могут быть неприемлимы для других проектов.
  *
  * mysqlxx нельзя рассматривать, как полноценную библиотеку, так как она разрабатывается
  *  из принципа "всё, что не используется сейчас - не реализовано".
  * Предполагается, что пользователь сам допишет недостающий функционал.
  */

#endif
