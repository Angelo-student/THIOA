function result = shift(wolf)
    len=length(wolf);
    % 定义整数范围
    min_val = 1;
    max_val = len;
    
    % 随机选取两个不同的整数
    random_integers = randperm(max_val - min_val + 1, 2) + min_val - 1;
    t=wolf(random_integers(1):random_integers(2));
    wolf(random_integers(1):random_integers(2))=[];
    result=[t,wolf];
end

