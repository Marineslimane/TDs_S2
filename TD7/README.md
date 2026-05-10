## Exercice 2 :

1. WeightedGraph est défini dans weightedGraph.hpp. Cette structure permet de représenter les noeuds du graphe avec leur poids. Elle permet d'obtenir les relations entre les différents noeuds. PositionedGraph, elle, est définie dans positionedGraph.hpp. Elle permet d'obtenir les coordonnées dans le plan des diférents noeuds afin de connaître leur emplacement. Dans sa définition, elle reprend donc la structure WeightedGraph car elle agit comme un complément à cette dernière. 

<br>

2. 
  - Extraction OSM : Ce module permet d'extraire les données d'OSM qui vont servir à représenter le graphe : nodes, ways, bounds. Avant cela, les données sont filtrées : les chemins formés par des constructions ou des chemins de fer ne sont pas pris en compte dans la représentation du graphe de ce code.
   - Simplification : Elle permet de réduire le nombre de noeuds du graphe et donc la précision de la représentation pour que cette dernière soit moins imposante. 
   - Visualisation : Elle permet de dessiner la représentation du graphe.


<br>

3. Étapes de simplification implémentées : 
   - keep_only_largest_connected_component : Elle ne garde que la composante connexe la plus grande du graphe ce qui permet de représenter la forme principale du plus grand "élement" de la carte. C'est un avantage car cela enlève des détails inutiles (composantes connexes inutiles car trop petites - on ne garde que la forme la plus importante) mais cela peut aussi être un inconvénient car on perd en précision dans la représentation 
    - remove_small_ending_edge : Elle supprime les arêtes dont la taille (distance entre les deux sommets qui la compose) est inférieure à une valeur donnée. L'avantage est qu'encore une fois, on enlève des détails inutiles car si la distance est très petite par rapport à l'échelle de la représentation du graphe, les sommets peuvent être confondus ou très proches donc on peut n'en garder qu'un. Toutefois, on perd encore en précision et si la valeur de la distance de référence est mal choisie, on peut très vite avoir des représentations complétement éronnées/pas du tout fidèles. 
   - remove_degree_two_nodes_by_angle_threshold : Elle permet de retirer les noeuds de degré 2 (ie reliés à 2 sommets distincts exactement) dont l'angle entre les deux arêtes partant de chacun de ces sommet est inférieur à une valeur d'angle donnée. Cela est un avantage car si l'angle est très petit alors les arêtes peuvent être confondues. Toutefois, encore une fois, si la valeur d'angle de référence est mal choisie alors on risque d'avoir des représentations éronnées. 
   - group_nodes_by_connection_depth_and_proximity : Elle permet de merge des noeuds qui sont relativement proches les uns des autres avec une méthode de clustering. L'avantage est que des noeuds proches peuvent être confondus si leur distance est très petite mais suivant la référence choisie, on perd encore une fois en précision. 