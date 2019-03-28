# Game

В игру играет 2 игрока. 

В начале происходит соединение: один игрок запускает
игру без параметров ждет надписи "TODO". Пример:

```TODO```

Второй игрок запускает игру с параметрами "TODO".
Например:

```TODO```

За соединение отвечает класс **GameSocket** (Singleton)

**********************************

Далее у вас будет возможность выбрать один из 4 народов:

*Воды(Water)

*Огня(Fire)

*Земли(Earth)

*Воздуха(Air)

У каждого из народов немного отличаются воины и лучники 
по характеристикам и цене и сильно отличаются маги

***************************

Далее для каждого игрока создается **Unit Factory** (Abstract Factory) 
его рассы и **CityFactory** и 3 начальных юнита:
**Colonist**, **Worker**, **Warrior**.

    # TODO:
    Также происходит генерация ресурсов на карте:
    * Золота
    * Железа
    * Дерева
    Которые будут добываться рабочими и переносится в город. (идея: Warcraft)
    За раз будет рабочий будет приносить 100 того ресурса что он добыл
    (В первой версии проекта в начале игры у вас есть 100500 каждого ресурса, для проверки системы генерации)

****************************

В игре юнитов можно создавать в городах. Создать воина **Warrior**
можно в любом городе, создать лучника **Archer** можно в городе с 
башней лучника **Archer Town**, мага **Wizard** только в городе с 
башней мага **Wizard Town**, котрая может быть создана только после 
создания башни лучника в данном городе.

***************************

Что можно создать в городе:

|Название|Время производства|Цена дерева|Цена железа|Цена золота| 
| --- | --- | --- | --- | --- |
|**Worker**| 2 хода| 50 | 10 | 0 |
|**Colonist**| 4 хода| 150 | 50 | 0 |
|**Warrior**| 2 хода| 50 | 50 | 10 |
|**Archer**| 3 хода| 100 | 50 | 20 |
|**Wizard**| 4 хода| 100 | 100 | 100 |
|**Archer Town**| 3 хода| 300 | 100 | 0 |
|**Wizard Town**| 3 хода| 300 | 300 | 100 |

***************************

Игра пошаговая. Первым ходит игрок запускавший игру без параметров.
Выигрывает тот кто захватил все города оппонента и убил всех юнитов.
Захват города происходит, когда военный юнит другого игрока заходит 
в этот город, а военный юнит может зайти на клетку только если там 
нет другого военного юнита (идея: Civilization)

Команды:

* Конец хода: ```Finish```
* Команда создания города: ```build [id_colonist]``` Например: ```build 0```
* Команда создание юнитов в городе: ```create [Unit_name] [city_id]```
Например: ```create Wizard 3``` или ```create Worker 8```
* Команда строительства башен в городе ```create [Tower_name] Tower [city_id]```
Например: ```create Archer Tower 1```

    
    # TODO:
    Нереализованные команды:
    * Перейти на соседнюю клетку юнитом
    * Аттака юнитом
    * Добыча ресурсов рабочим

Команды для тестирования работы игры:

* Убийство военного юнита: ```kill combat [unit_id]```. Например: ```kill combat 5```
* Убийство военного юнита: ```kill military [unit_id]```. Например: ```kill military 2```

*********************

Дополнительные правила:
* На каждой клетке может максим находится 1 военный юнит и 1 мирный