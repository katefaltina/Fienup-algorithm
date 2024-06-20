<p>Поскольку для применения алгоритма Фиенупа изначально должен быть известен модуль спектра, в качестве подготовительного шага предстоит получить модуль спектра некоторого известного сигнала, модуль спектра которого будет служить входными данными алгоритма. В этой задаче был сгенерирован сигнал, состоящий из трёх гауссовых куполов.</p>
<p>! Для получения лучшего результата рекомендуется брать достаточно узкие купола, не перекрывающиеся друг с другом.</p>
<p>Вычислив модуль преобразования Фурье этого сигнала, получим модуль спектра, фазу которого в дальнейшем требуется восстановить с помощью алгоритма Фиенупа.</p>
<ul>Итерационная схема алгоритма состоит из следующих шагов:
<li><p>1. Инициализация:</p>
Из известного модуля строится комплексная последовательность чисел, имеющихся известный модуль и случайную фазу 𝜙0 с равномерным распределением в интервале [0, 2𝜋]. Использование именно равномерно распределённой случайной фазы значительно повышает вероятность получить начальное приближение сигнала с нужными для реализации алгоритма восстановления свойствами.</li>
<li><p>2. Применение ограничений во временной области:</p>
Поскольку априорно известно, что исходный сигнал представляет собой сумма гауссовых куполов и является положительно определённым, то можем восстановить сигнал по построенному комплексному спектру и во временной области применить ограничение на неотрицательность сигнала.</li>
<li><p>3. Замена модуля в частотной области на известный:</p>
Применение ограничения на неотрицательность во временной области влечёт за собой определённые изменения в спектральной области, в том числе модуля спектра. Из-за этого встаёт необходимость замены модуля на известный модуль спектра |𝑆|. Можно заменить модуль комплексного числа только с помощью простых арифметических операций. При этом нет необходимости пользоваться тригонометрическими функциями для вычисления фазы.</li>
<li><p>4. Проверка сходимости итерационной схемы:</p> 
Повторяются шаги (2) и (3) пока изменения сигнала не перестали превышать заданный порог среднеквадратичного отклонения на один отсчёт.</li>
</p>
<img src="https://github.com/katefaltina/Fienup-algorithm/assets/125446752/72cd7972-203b-4518-909b-45b13449c4fb">
