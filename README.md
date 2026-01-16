# THIOA:A Three-Phase Hybrid Optimization for Curvature-Constrained Laser Drilling with AODs Galvanometer Systems

The integration of rapid-scanning Acousto-Optic Deflectors (AODs) with large-range
galvanometers presents a critical path planning challenge in laser drilling. The core
problem is to fully exploit the AODs’s rectangular field-of-view for efficient coverage
while adhering to the galvanometer’s stringent kinematic constraints. Traditional Trav-
eling Salesman Problem (TSP) models are inadequate, as they fail to model the AODs’s
coverage capability and ignore essential smoothness and curvature constraints. This pa-
per introduces a Three-phase Hybrid Intelligent Optimization Architecture (THIOA)
to bridge this gap. First, THIOA converts the problem from visiting individual points
to covering regions by employing a greedy point set packing algorithm that clusters
drilling points into optimally positioned rectangles. Subsequently, the global visita-
tion sequence is optimized using the Genetic Algorithm with Edge Assembly Crossover
(GA-EAX). Third, and most critically, a local B-spline path is generated under a dual-
objective optimization framework to guarantee C2 continuity and strict curvature com-
pliance. Moreover, THIOA can efficiently manage scenarios involving tens of thousands
of drilling points—reducing computational costs without compromising path quality.
Extensive experiments demonstrate that THIOA significantly shortens the path length
while strictly adhering to industrial curvature constraints. Notably, it outperforms
state-of-the-art like GA-EAX algorithm at higher point densities. On the 55 test cases
of TSPLIB, the average path length was reduced by 8.6% in high-density scenarios.


This code was developed on Matlab 2023b and requires the parallel acceleration package of Matlab. For solving the tsp problem, we used the ga-eax algorithm.
