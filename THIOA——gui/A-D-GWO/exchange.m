function result=exchange(ga_o, wolf)
% ga_o为操作集，wolf为一个个体,ga_0第一列为位置，第二列为该位置alpha狼的基因
    if isempty(ga_o)
        result=wolf;
        return
    end
    len_o=size(ga_o,1);
    random_integer = randi([1, len_o]); %随机整数
    set_index = randsample(1:len_o, random_integer);
    
    for i=1:size(set_index,1)
        data=ga_o(set_index(i),:);
        index_t=find(wolf==data(2));

        wolf(index_t)=wolf(data(1));
        wolf(data(1))=data(2);
        

    end
    result=wolf;
    
end