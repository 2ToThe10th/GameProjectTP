# Game

### [![Build Status](https://travis-ci.org/2ToThe10th/Game.svg?branch=dev)](https://travis-ci.org/2ToThe10th/Game)

Для того чтобы запустить тесты для проверки создания юнитов и городов,
воспользуйтесь командами:

```cmake -DDEBUG=1```
       
```make```
       
```./game```

В игру играет 2 игрока. 

В начале происходит соединение:
 
один игрок запускает
игру и на вопрос "Do you want to create a server or connect to exist?[S/C]" 
отвечает "S" или "s". Потом выбирает порт для открытия сервера.
 
Второй игрок запускает игру и на тот же вопрос отвечает "C" или "c". После этого
выбирает hostname компьютера первого игрока (localhost, если играть с одного 
компьютера) и порт выбранный ранее.

За соединение отвечает класс **GameSocket** (Singleton)

**********************************

Далее у вас будет возможность выбрать один из 4 народов:

*Воды(Water)

*Огня(Fire)

*Земли(Earth)

*Воздуха(Air)

У каждого из народов немного отличаются воины и лучники 
по характеристикам и сильно отличаются маги

***************************

Далее для каждого игрока создается **Unit Factory** (Abstract Factory) 
его рассы и **CityFactory** и 2 начальных юнита:
**Worker**, **Warrior**.

Также происходит генерация ресурсов на карте:
* Золота
* Железа
* Дерева

Которые будут добываться рабочими и переносится в город. (идея: Warcraft)
За раз будет рабочий будет приносить 100 того ресурса что он добыл

****************************

В игре юнитов можно создавать в городах. Создать воина **Warrior**
можно в любом городе, создать лучника **Archer** можно в городе с 
башней лучника **Archer Town**, мага **Wizard** только в городе с 
башней мага **Wizard Town**, котрая может быть создана только после 
создания башни лучника в данном городе.

***************************

Что можно создать в городе:

|Название|Цена дерева|Цена железа|Цена золота| 
| --- | --- | --- | --- |
|**Worker**| 50 | 10 | 0 |
|**Colonist**| 150 | 50 | 0 |
|**Warrior**| 50 | 50 | 10 |
|**Archer**| 100 | 50 | 20 |
|**Wizard**| 100 | 100 | 100 |
|**Archer Town**| 300 | 100 | 0 |
|**Wizard Town**| 300 | 300 | 100 |

***************************

Игра пошаговая. Первым ходит игрок запускавший игру без параметров.
Выигрывает тот кто захватил все города оппонента(после захвата всех 
городов нужно написать ```End```).
Захват города происходит, когда военный юнит другого игрока заходит 
в этот город, а военный юнит может зайти на клетку только если там 
нет другого военного юнита (идея: Civilization)
    
Команды:
* Создать город на месте где стоит колонист ```BuildCity {id_colonist}```
* Создать юнита в городе ```CreateUnit {Colonist/Worker/Warrior/Archer/Wizard} {city_id}```
* Построить башню в городе ```CreateTower {Archer/Wizard} {city_id}```
* Конец хода ```End```
* Добыча ресурсов рабочим ```Mine {worker_id}``` (после добычи нужно прийти в город, 
тогда этими ресурсами можно будет пользоватся, пока рабочий несет ресурсы он не может добывать другие)
* Перейти на соседнюю клетку юнитом ```Go {Combat/Worker/Colonist} {unit_id} {where}```
* Атака юнитом ```Attack {combat_unit_id} {where}```
    
    
    где where это строка из символов u(Up), l(Left), r(Right), d(Down)
    Например: "Attack {unit_id} ul" обозначает атаковать клетку сверху слева от юнита

*********************

Дополнительные правила:
* На каждой клетке может максимум находится 1 военный юнит и 1 мирный 
и не могут находится юниты разных игроков
* Если военный юнит переходит на клетку с мирным юнитом другого игрока, 
то мирный юнит меняет свою сторону
* Если военный юнит переходит на клетку с чужим городом,
то город меняет свою сторону