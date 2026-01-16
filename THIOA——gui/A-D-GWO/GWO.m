function [Best_Pos,Best_Score]=GWO(pop,citys,max_iter)

    num_citys=length(citys);  % 城市数量
    distance_matrix = calculate_distance_matrix(citys);
    x=initialization(pop,num_citys);  %初始化种群
    Fitness=zeros(1,pop);%初始化适应度函数
    for i=1:pop
        Fitness(i)=getfitness(x(i,:),distance_matrix);
    end
    [~,IndexSort]=sort(Fitness);
    Alpha_Pos=x(IndexSort(1),:);
    Beta_Pos=x(IndexSort(2),:);
    Delta_Pos=x(IndexSort(3),:);
    wolf_leader=[Alpha_Pos;Beta_Pos;Delta_Pos];
    %------------------------------------------------------------
    F_av=[];
    for t=1:10
        t
        a=1-t/10;
       for i=1:pop
            [fit,rou]=updata(x(i,:),wolf_leader,a,distance_matrix);
            x(i,:)=rou;
            Fitness(i)=fit;
       end
       [~,IndexSort]=sort(Fitness);
        Alpha_Pos=x(IndexSort(1),:);
        Beta_Pos=x(IndexSort(2),:);
        Delta_Pos=x(IndexSort(3),:);
        wolf_leader=[Alpha_Pos;Beta_Pos;Delta_Pos];
        av=mean(Fitness);
        F_av=[F_av,av];
    end
    %-------------------------------------------------------------
    a=1;
    F_w=mean(F_av);
    count=10;
    while a>0.01
        count=count+1
        for i=1:pop
            [fit,rou]=updata(x(i,:),wolf_leader,a,distance_matrix);
            x(i,:)=rou;
            Fitness(i)=fit;
       end
       [SortFitness,IndexSort]=sort(Fitness);
        Alpha_Pos=x(IndexSort(1),:);
        Beta_Pos=x(IndexSort(2),:);
        Delta_Pos=x(IndexSort(3),:);
        wolf_leader=[Alpha_Pos;Beta_Pos;Delta_Pos];
        av=mean(Fitness);
        F_av(1)=[];
        F_av=[F_av,av];
        F_w=mean(F_av);
        if av>F_w
            a=0.9*a;
        else
            a=a-1/max_iter;
        end
        if count>=max_iter
            break
        end
    end
    
    Alpha_Score=SortFitness(1);
    Best_Pos=Alpha_Pos;
    Best_Score=Alpha_Score;
end


function distance_matrix = calculate_distance_matrix(coordinates)
    num_cities = size(coordinates, 1);
    distance_matrix = zeros(num_cities, num_cities);
    for i = 1:num_cities
        for j = 1:num_cities
            distance_matrix(i, j) = norm(coordinates(i, :) - coordinates(j, :));
        end
    end
end