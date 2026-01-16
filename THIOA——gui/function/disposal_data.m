function [original_data,limit_t] = disposal_data(circletab,gatherpoint,data)
    len=length(circletab);
    depoints=[];
    centers=[];
    radiums=[];
    
    for i=1:len
        depoints=[depoints,gatherpoint{i}];
        data_t=circletab{i};
        centers=[centers;data_t(1:2)];
        radiums=[radiums;data_t(3:4)];
    end
    data(depoints,:)=[];
    
    original_data=[data;centers];
    
    limit_t = [zeros(size(data));radiums];  % 每个点的限制
end

