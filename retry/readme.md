2. beadandó feladat. Beadási határidő: 2018-12-6. Készítsen C programot amely megoldja a következő feladatot:

"Fényes Nap Kft" (szülő) törekszik a rendelések telepítését hatékonyan végezni. Két azonos teljesítményű rendszer telepítése egy napi munkát jelent egy szerelő csapatnak (gyerek). Ugyanakkor ha rendelés felvételi ideje óta legalább egy hét eltelt, akkor ezt a rendelést akár napi egy feladatként, soron kívül is kiadja egy telepítő brigádnak. A napi feladatokat, azok adatait a társaság (szülő) csővezetéken küldi a szerelő(k)nek (gyerek), aki(k) egy "nyugtával" jelzik, hogy vették az egy vagy két telepítést tartalmazó feladatot. A szerelők miután üzembe helyeztek egy rendszert, tudatják a központtal (jelzéssel), hogy a rendszer működik. Ha a szerelés az aznapi utolsó szerelési feladat volt, akkor a szerelő a napi munkalap zárásaként üzenetet (csővezetéken) küld a megbízónak, hogy a napi feladatokat(ami egy vagy két szerelés volt) megoldotta. Ezen üzenet hatására "Fényes Nap Kft" bejegyzi, hogy ezek a rendelések teljesítve vannak!

Készítsen C programot, amellyel a leírt tevékenység elvégezhető.

Szereplők:

Társaság (szülő)
^
| ^^ Nyugta hogy megkapta a feladatot
pipe
| ˇˇFeladat: 1 vagy 2 telepítés
ˇ
Szerelő (gyerek)
^
|
szemafor
|
ˇ
Központ

sem_lock(sem_worker_guard, sem_name_sem_worker_guard);
sem_release(sem_worker_guard, sem_name_sem_worker_guard);

sem_lock(sem_company_guard, sem_name_sem_company_guard);
sem_release(sem_company_guard, sem_name_sem_company_guard);

sem_lock(sem_pipe_guard, sem_name_sem_pipe_guard);
sem_release(sem_pipe_guard, sem_name_sem_pipe_guard);