% 这个函数在每次删去子集中的点后都将该子集放回原来，再挑选一个能够子集中点集数量最多的点进行覆盖

function [circletab,gatherpoints] = fun_gatherpoints(data, thr)
  %输入点集数据，每一行为一个点 thr为距离阈值
  %circletab为圆心坐标和半径表， gatherpoints为每组中点序号
    result_t = find_nearby_points_kd(data, sqrt(2)*thr);
    %按照元素个数从高到低排序
    vectorLengths = cellfun(@numel, result_t);
    [~, lengthIndices] = sort(vectorLengths,'descend');
    result = result_t(lengthIndices);

    len=length(data);

    usedlist=[];  %记录已经用过的点
    circletab={};   %记录矩形的中心和长宽
    gatherpoints={};  %记录被聚集的点
    
    % 只要还有点集需要处理就继续循环
    while ~isempty(result) && ~isempty(result{1})
        index_t=result{1};  % 取点数最多的子集
        result(1) = [];     % 从结果列表中移除
        
        index_t=setdiff(index_t, usedlist); %获取该点周围没使用过的近邻点
        len_data=length(index_t);
        
        if len_data<=1   %如果该点距离周围点都超过阈值，跳过
            continue;
        else  %该点周围有多个点
            % 依次去除距离其余点最远的点
            current_subset = index_t;
            while true
                data_t=data(current_subset,:);
                [center, len, d] = min_cover_squrte(data_t(:,1), data_t(:,2));
                
                if len<=thr && d<=thr
                    % 符合条件，添加到结果
                    circletab{end+1}=[center,len, d];
                    gatherpoints{end+1}=current_subset;
                    usedlist=[usedlist,current_subset];
                    % 更新结果列表，移除已使用的点
                    result = update_result(result, usedlist);
                    break;
                else
                    % 不符合条件，删除最远点
                    distMatrix = pdist2(data_t, data_t);
                    [~, maxindex]=max(sum(distMatrix));
                    
                    % 保存要删除的点
                    removed_point = current_subset(maxindex);
                    current_subset(maxindex)=[];
                    
                    if length(current_subset)<=1
                        % 如果剩余点太少，将当前子集放回结果列表
                        if ~isempty(current_subset)
                            result{end+1} = current_subset;
                            result = sort_result(result); % 重新排序
                        end
                        break;
                    else
                        % 将修改后的子集放回结果列表并重新排序
                        result{end+1} = current_subset;
                        result = sort_result(result); % 重新排序
                        % 取下一个最大的子集
                        current_subset = result{1};
                        result(1) = [];
                        current_subset = setdiff(current_subset, usedlist);
                        
                        if length(current_subset)<=1
                            break;
                        end
                    end
                end
            end
        end
    end
end

function result = update_result(result, used)
    % 更新结果列表，移除已使用的点
    len=length(result);
    for i=1:len
        result{i}=setdiff(result{i}, used);
    end
    % 过滤空集并重新排序
    result = result(~cellfun('isempty', result));
    result = sort_result(result);
end

function result = sort_result(result)
    % 按照元素个数从高到低排序结果列表
    if isempty(result)
        return;
    end
    vectorLengths = cellfun(@numel, result);
    [~, lengthIndices] = sort(vectorLengths,'descend');
    result = result(lengthIndices);
end

function [center, len, d]=min_cover_squrte(x, y)
    % 返回矩阵中心点，长和宽
    len=max(x)-min(x);
    d=max(y)-min(y);
    center=[(max(x)+min(x))/2, (max(y)+min(y))/2];
end