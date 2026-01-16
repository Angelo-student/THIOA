function [tour,len] = eax(current_path,original_data)
    [n, m]=size(original_data);
    if n <= 4
        if n==0 || n==1
            len=0;
            tour=[];
        elseif n==2
            len=2*norm(original_data(1,:)-original_data(2,:));
            tour=[1, 2];
        elseif n==3
            len=norm(original_data(1,:)-original_data(2,:))+norm(original_data(2,:)-original_data(3,:))+norm(original_data(1,:)-original_data(3,:));
            tour=[1, 2, 3];
        else
            permutations = {
            [1,2,3,4];  % 顺序连接
            [1,2,4,3];  % 第二种连接方式
            [1,3,2,4]   % 第三种连接方式
            };
            
            % 计算每种连接方式的总边长方差（方差小表示边长按近更均匀）
            variances = zeros(3,1);
            for i = 1:3
                idx = permutations{i};
                s = [norm(original_data(idx(2),:)-original_data(idx(1),:)); 
                     norm(original_data(idx(3),:)-original_data(idx(2),:)); 
                     norm(original_data(idx(4),:)-original_data(idx(3),:)); 
                     norm(original_data(idx(1),:)-original_data(idx(4),:))];
                variances(i) = sum(s);
            end
            
            [len, best_idx] = min(variances);
            tour = permutations{best_idx};
        end
        return
    end
    output_file = fullfile(current_path, 'Large/position.tsp'); % 输出的TSP文件
    % 生成TSP文件
    create_tsp_file(output_file, original_data);
    
    % 定义路径和参数
    code_dir = fullfile(current_path, 'Large');
    
    %cmd1 = ['cd /d ', code_dir, ' && '...
    % 'g++ -o jikken -O3 main.cpp env.cpp cross.cpp evaluator.cpp indi.cpp...
    % rand.cpp kopt.cpp sort.cpp -lm',...
    % ' && ', 'jikken.exe 1 position 100 30 position.tsp >nul 2>&1'];
    
    max_attempts = 5;
    success = false;
    filename = fullfile(code_dir, 'position_BestSol');
    for attempt = 1:max_attempts
        % 构建命令字符串
        cmd = sprintf('cd /d "%s" && jikken.exe 1 position 100 30 position.tsp >nul 2>&1', code_dir);
        % 执行命令
        system(cmd);
        % 检查文件是否存在
        if exist(filename, 'file') == 2
            success = true;
            fprintf('第%d次尝试成功，文件已生成\n', attempt);
            break;
        else
            fprintf('第%d次尝试失败，文件未生成，将重试...\n', attempt);
        end
    end
    
    if ~success
        error('经过%d次尝试后仍未生成文件position_BestSol', max_attempts);
    end
    % pause(2);  % 程序暂停2秒

    fid = fopen(filename);
    t = str2num(fgetl(fid));
    len=t(2);
    second_line = fgetl(fid);
    fclose(fid);
    tour = str2num(second_line);
    
    delete(fullfile(code_dir, 'position_BestSol'));
    delete(fullfile(code_dir, 'position_Result'));
    delete(fullfile(code_dir, 'position.tsp'));

    len=len/1000;
end

