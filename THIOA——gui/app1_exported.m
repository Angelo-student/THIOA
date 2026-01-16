classdef app1_exported < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure           matlab.ui.Figure
        Button_10          matlab.ui.control.StateButton
        excelButton        matlab.ui.control.StateButton
        Label_6            matlab.ui.control.Label
        Label_5            matlab.ui.control.Label
        ADGWOButton        matlab.ui.control.Button
        resultTable        matlab.ui.control.Table
        Label_4            matlab.ui.control.Label
        PDFButton          matlab.ui.control.Button
        CDFButton          matlab.ui.control.Button
        Button_3           matlab.ui.control.Button
        Text_importdata    matlab.ui.control.TextArea
        THIOAButton        matlab.ui.control.Button
        DropDown           matlab.ui.control.DropDown
        Button_2           matlab.ui.control.Button
        yEditField         matlab.ui.control.NumericEditField
        yEditFieldLabel    matlab.ui.control.Label
        xEditField         matlab.ui.control.NumericEditField
        xEditFieldLabel    matlab.ui.control.Label
        Label_3            matlab.ui.control.Label
        Packing            matlab.ui.control.Button
        GaeaxButton        matlab.ui.control.Button
        Label_2            matlab.ui.control.Label
        minradio           matlab.ui.control.NumericEditField
        EditFieldLabel     matlab.ui.control.Label
        Text_result        matlab.ui.control.TextArea
        Label              matlab.ui.control.Label
        import_button      matlab.ui.control.Button
        AODEditField       matlab.ui.control.NumericEditField
        AODEditFieldLabel  matlab.ui.control.Label
        Button_6           matlab.ui.control.StateButton
        Button_7           matlab.ui.control.StateButton
        Button_8           matlab.ui.control.StateButton
        Button_9           matlab.ui.control.Button
        ax_pack            matlab.ui.control.UIAxes
        ax_cdf             matlab.ui.control.UIAxes
        ax_pdf             matlab.ui.control.UIAxes
        ax_path            matlab.ui.control.UIAxes
    end

    
    properties (Access = private)
        Data % 用于存储原始的点集数据
        thr % AOD视场阈值
        min_r % 最小转弯半径
        cu_path % 记录当前app1所在的文件夹
        tour_eax % eax算法求得的路径访问顺序
        len_eax % eax算法求得的路径长度
        x_lim % 线性缩放后x方向上极值
        y_lim %线性缩放后y方向上的极值
        data_packed % 聚点后的点集数据
        plimit  %聚点后点集的活动范围
        circletab %存储打包的点集以及巨大矩形大小的信息
        len_de  % 被删去点的打孔长度
        sumlen % 总路径长度
        opt_len  % 优化后的曲线部分路径长度
        opt_poi  % 优化后点的位置
        de_ind    % 被删去点的索引
        tor_eaxed   %  按照eax算法求得的路径访问顺序排列的点坐标，用于THIOA中
        num_im    %导入文件的数量
        ispacked  % 是否打包过了
        isTHIOA   % 是否运行了THIOA算法
    end
    
    methods (Access = private)
        
    end
    

    % Callbacks that handle component events
    methods (Access = private)

        % Code that executes after component creation
        function startupFcn(app)
            % 生成app后自动运行，用于初始化
            app.cu_path = fileparts(mfilename('fullpath')); 
            addpath(fullfile(app.cu_path, 'function')); 
            addpath(fullfile(app.cu_path, 'Large'));
            addpath(fullfile(app.cu_path, 'A-D-GWO'));
            if isempty(gcp('nocreate'))
                parpool; % 启动并行池
            end

            % 参数初始化
            app.num_im=0;
            app.ispacked=0;
            app.isTHIOA=0;
            app.AODEditField.Value=0.32;
            app.thr=0.32;
            app.minradio.Value=0.06;
            app.min_r=0.06;
            app.ax_path.Position=[365, 241, 428, 362];
            app.ax_pack.Position=[761, 716, 428, 362]; app.ax_pdf.Position=[761, 716, 428, 362];
            app.ax_cdf.Position=[761, 716, 428, 362];
            app.resultTable.Data=["请导入文件","请导入文件","未运算","未运算","未运算","未运算","未运算"];
        end

        % Button pushed function: import_button
        function import_buttonPushed(app, event)
            % 导入数据的函数
            % 定义可选择的文件类型
            fileTypes = {'*.txt';'*.xlsx';'*.csv';'*.tsp'};

            [fileName, filePath] = uigetfile(fileTypes, '选择文件');
            if fileName == 0
                return; % 取消操作，退出函数
            end
            % 拼接完整文件路径
            fullFilePath = fullfile(filePath, fileName);
            
            app.Text_importdata.Value = ['已选择文件：', fullFilePath];
            
            [~, ~, fileExt] = fileparts(fullFilePath); % 获取文件扩展名
            try
                switch lower(fileExt)
                    case {'.xlsx', '.csv','.txt'}
                        app.Data = readmatrix(fullFilePath);
                        app.num_im=app.num_im+1;
                        msg = sprintf('成功读取文件，第%d个文件',app.num_im);
                        app.resultTable.Data(app.num_im,:)=["请导入文件","请导入文件","未运算","未运算","未运算","未运算","未运算"];
                    case '.tsp'
                        [~, city_position] = readTSPFile(fullFilePath);
                        app.Data = city_position;
                        app.num_im=app.num_im+1;
                        msg = sprintf('成功读取文件，第%d个文件',app.num_im);
                        % 使用圆括号提取出一整行
                        app.resultTable.Data(app.num_im,:)=["请导入文件","请导入文件","未运算","未运算","未运算","未运算","未运算"];
                    otherwise
                        msg = '文件类型暂不支持读取';
                        app.Text_importdata.Value = msg;
                        return;
                end
           catch err
                app.Text_importdata.Value = ['读取失败：', err.message];
                app.Text_importdata.FontColor = [0,0,0];
                return;
            end
                
            app.Text_importdata.Value = msg;
            app.Text_importdata.FontColor = [0,0,0];
            app.x_lim = max(app.Data(:,1)) - min(app.Data(:,1));
            app.y_lim = max(app.Data(:,2)) - min(app.Data(:,2));
            app.xEditField.Value = app.x_lim;
            app.yEditField.Value = app.y_lim;
            app.resultTable.Data{app.num_im,1}=fileName;
            app.resultTable.Data{app.num_im,2}=num2str(size(app.Data,1));
            
        end

        % Value changed function: minradio
        function minradioValueChanged(app, event)
            % 给最小转弯半径赋值
            value = app.minradio.Value;
            app.min_r = value;
        end

        % Value changed function: AODEditField
        function AODEditFieldValueChanged(app, event)
            % 给AOD视场阈值赋值
            value = app.AODEditField.Value;
            app.thr = value;
        end

        % Button pushed function: GaeaxButton
        function GaeaxButtonPushed(app, event)
            % 按下按钮使用ga-eax算法计算结果并输出路径长度
            if isempty(app.Data)
                uialert(app.UIFigure, '请先导入数据', '提示');
                return
            end
            [app.tour_eax,app.len_eax] = eax(app.cu_path,app.Data);
            app.resultTable.Data{app.num_im,6}=num2str(app.len_eax);
            resultInfo = sprintf('第%d个文件利用ga-eax算法求解结果：%.4f', app.num_im,app.len_eax);
            app.Text_result.Value = [app.Text_result.Value; newline; resultInfo];
            scroll(app.Text_result,"bottom")
        end

        % Value changed function: xEditField
        function xEditFieldValueChanged(app, event)
            % 对数据进行线性缩放，x方向上极值
            value = app.xEditField.Value;
            app.x_lim = value;
        end

        % Value changed function: yEditField
        function yEditFieldValueChanged(app, event)
            % 对数据进行线性缩放，y方向上极值
            value = app.yEditField.Value;
            app.y_lim = value;
        end

        % Button pushed function: Packing
        function PackingPushed(app, event)
            % 点集打包算法按钮，按下执行点集打包操作
            if isempty(app.Data)
                uialert(app.UIFigure, '请先导入数据', '提示');
                return
            end
            % 首先进行点集放缩
            app.Data = scaleTomn(app.Data, app.x_lim/2, app.y_lim/2);
            [app.circletab,gatherpoint]=fun_gatherpoints(app.Data, app.thr);
            resultInfo = sprintf('点集打包算法已运行完毕');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
            drawnow
            %. 整理数据并导出
            [app.data_packed,app.plimit] = disposal_data(app.circletab,gatherpoint,app.Data);
            app.resultTable.Data{app.num_im,3}=num2str(size(app.data_packed,1));
            resultInfo = sprintf('打包后的数据已整理完毕');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
            app.ispacked=app.num_im;
        end

        % Button pushed function: Button_2
        function Button_2Pushed(app, event)
            % 画图 打包后点集效果提
            if app.ispacked ~= app.num_im
                uialert(app.UIFigure, '请先运行打包算法', '提示');
                return
            end
            Packing_plot(app.ax_pack, app.Data, app.circletab)
            resultInfo = sprintf('点集打包效果图绘制成功，请通过下拉框选择后查看');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
        end

        % Button pushed function: THIOAButton
        function THIOAButtonPushed(app, event)
            % 执行THIOA算法主体部分
            resultInfo = sprintf('正在进行TSP问题求解阶段');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
            drawnow
            if isempty(app.Data)
                uialert(app.UIFigure, '请先导入数据', '提示');
                return
            end
            if app.ispacked ~= app.num_im
                uialert(app.UIFigure, '请先运行打包算法', '提示');
                return
            end

            [tour,~] = eax(app.cu_path,app.data_packed);
            
            resultInfo = sprintf('TSP问题已求解完毕');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
            drawnow
            % 相关参数与数据的准备
            original_points = app.data_packed;
            original_points = original_points(tour,:);
            app.tor_eaxed = original_points;
            % 每个点的矩形范围 [xmin, xmax, ymin, ymax]
            limit = app.plimit;
            limit = limit(tour,:);
            rect_bounds = [original_points(:,1)-(app.thr-limit(:,1))/2,original_points(:,1)+(app.thr-limit(:,1))/2,original_points(:,2)-(app.thr-limit(:,2))/2,original_points(:,2)+(app.thr-limit(:,2))/2];
            % 序贯优化第一步，优化点的数量
            resultInfo = sprintf('正在进行序贯优化第一阶段');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
            drawnow

            [opt_points,current_bounds, original_indices, deleted_indices] = optim_numofpoint(original_points,...
                rect_bounds, 1/app.min_r);

            resultInfo = sprintf('第一阶段结束，尝试对弃置的点进行放回');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
            drawnow
            % 补偿阶段，尝试将被删去的点放回
            [current_points,current_bounds,app.de_ind] = compensation_fun(opt_points,current_bounds,original_indices,...
                deleted_indices, original_points, rect_bounds,1/app.min_r);

            app.resultTable.Data{app.num_im,4}=num2str(size(app.data_packed,1)-size(current_points,1));

            % 序贯优化第二步，路径长度优化 
            resultInfo = sprintf('正在进行序贯优化第二阶段');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
            drawnow
            [app.opt_poi, app.opt_len] = optim_distance(current_points,current_bounds,1/app.min_r);
            % 最终距离计算
            de_poi = original_points(app.de_ind,:);
            [~, app.len_de] = eax(app.cu_path, de_poi);
            app.sumlen = app.opt_len+app.len_de;
            app.resultTable.Data{app.num_im,5}=num2str(app.sumlen);
            % fprintf('总长度: %.2f\n', sumlen);
            app.isTHIOA=app.num_im;
            resultInfo = sprintf('THIOA算法结束，曲线部分长度为:%.4f，被弃置点部分长度为:%.4f, 总长度为:%.4f', app.opt_len, app.len_de, app.sumlen );
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
        end

        % Button pushed function: Button_3
        function Button_3Pushed(app, event)
            % 路径图
            if app.isTHIOA ~= app.num_im
                uialert(app.UIFigure, '请先运行THIOA算法', '提示');
                return
            end
            result_path_plot(app.ax_path, app.tor_eaxed, app.opt_poi, app.de_ind)
            resultInfo = sprintf('路径对比图绘制成功，请通过下拉框选择后查看');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
        end

        % Button pushed function: CDFButton
        function CDFButtonPushed(app, event)
            % 对数曲率累计概率
            if app.isTHIOA ~= app.num_im
                uialert(app.UIFigure, '请先运行THIOA算法', '提示');
                return
            end
            plotCurvatureCDF(app.ax_cdf, app.tor_eaxed, app.opt_poi, 1/app.min_r)
            resultInfo = sprintf('对数曲率累计概率曲线图绘制成功，请通过下拉框选择后查看');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
        end

        % Button pushed function: PDFButton
        function PDFButtonPushed(app, event)
            % 对数曲率概率密度
            if app.isTHIOA ~= app.num_im
                uialert(app.UIFigure, '请先运行THIOA算法', '提示');
                return
            end
            plotCurvaturePDF(app.ax_pdf, app.tor_eaxed, app.opt_poi, 1/app.min_r)
            resultInfo = sprintf('对数曲率概率密度曲线图绘制成功，请通过下拉框选择后查看');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
        end

        % Value changed function: DropDown
        function DropDownValueChanged(app, event)
            value = app.DropDown.Value;
            if value == "路径图"
                app.ax_path.Position=[365, 241, 428, 362];
                app.ax_pack.Position=[761, 716, 428, 362]; app.ax_pdf.Position=[761, 716, 428, 362];
                app.ax_cdf.Position=[761, 716, 428, 362];
            elseif value == "累计概率曲线"
                app.ax_cdf.Position=[365, 241, 428, 362];
                app.ax_pack.Position=[761, 716, 428, 362]; app.ax_pdf.Position=[761, 716, 428, 362];
                app.ax_path.Position=[761, 716, 428, 362];
            elseif value == "打包算法效果图"
                app.ax_pack.Position=[365, 241, 428, 362];
                app.ax_path.Position=[761, 716, 428, 362]; app.ax_pdf.Position=[761, 716, 428, 362];
                app.ax_cdf.Position=[761, 716, 428, 362];
            else
                app.ax_pdf.Position=[365, 241, 428, 362];
                app.ax_pack.Position=[761, 716, 428, 362]; app.ax_path.Position=[761, 716, 428, 362];
                app.ax_cdf.Position=[761, 716, 428, 362];
            end
        end

        % Value changed function: excelButton
        function excelButtonValueChanged(app, event)
            % 1. 获取表头（列名）和表格数据
            header = app.resultTable.ColumnName;  % 表头（如{'ID', 'Name', 'Score'}）
            tableData = app.resultTable.Data;     % 表格数据（table或cell类型）
            
            % 2. 合并表头和数据（确保表头作为第一行）
            headerCell = header;  % 表头本身是cell数组
            % 合并表头和数据
            mergedData = [headerCell'; tableData];
            
            % 3. 让用户选择保存路径
            [fileName, filePath] = uiputfile(...
                {'*.xlsx', 'Excel Files (*.xlsx)'; '*.xls', 'Excel 97-2003 Files (*.xls)'}, ...
                '导出表格');
            if fileName == 0  % 用户取消选择
                return;
            end
            fullPath = fullfile(filePath, fileName);  % 完整保存路径
            
            % 4. 导出到Excel
            try
                writematrix(mergedData, fullPath, 'Sheet', '数据汇总');  % 可指定工作表名
                uiconfirm(app.UIFigure, ...
                            '表格已成功导出！', ...  % 提示内容
                            '导出成功', ...                  % 标题
                            'Icon', 'success', ...          % 图标：绿色对勾
                            'Options', '确定');                % 仅显示“OK”按钮
            catch err
                uialert(app.UIFigure, ['导出失败：', err.message], '错误提示');
            end
        end

        % Value changed function: Button_10
        function Button_10ValueChanged(app, event)
            
            if isempty(app.Data)
                uialert(app.UIFigure, '请先导入数据', '提示');
                return
            end
            % 一键运行代码
            PackingPushed(app, event);
            THIOAButtonPushed(app, event);
            GaeaxButtonPushed(app, event);
            ADGWOButtonPushed(app, event);
             resultInfo = sprintf('所有算法运行结束，可以绘图查看结果或进行下一个文件的测试');
            app.Text_result.Value = [app.Text_result.Value; newline; {resultInfo}];
            scroll(app.Text_result,"bottom")
        end

        % Button pushed function: ADGWOButton
        function ADGWOButtonPushed(app, event)
            if isempty(app.Data)
                uialert(app.UIFigure, '请先导入数据', '提示');
                return
            end
            pop=20;
            MaxIter=1000;
            [~,len_GWO]=GWO(pop, app.Data,MaxIter);
            app.resultTable.Data{app.num_im,7}=num2str(len_GWO);
            resultInfo = sprintf('第%d个文件利用A-D-GWO算法求解结果：%.4f', app.num_im,len_GWO);
            app.Text_result.Value = [app.Text_result.Value; newline; resultInfo];
            scroll(app.Text_result,"bottom")
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create UIFigure and hide until all components are created
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.Position = [100 100 827 652];
            app.UIFigure.Name = 'MATLAB App';

            % Create ax_path
            app.ax_path = uiaxes(app.UIFigure);
            zlabel(app.ax_path, 'Z')
            app.ax_path.XTick = [];
            app.ax_path.YTick = [];
            app.ax_path.NextPlot = 'replace';
            app.ax_path.Position = [365 241 428 362];

            % Create ax_pdf
            app.ax_pdf = uiaxes(app.UIFigure);
            title(app.ax_pdf, 'Title')
            xlabel(app.ax_pdf, 'X')
            ylabel(app.ax_pdf, 'Y')
            zlabel(app.ax_pdf, 'Z')
            app.ax_pdf.NextPlot = 'replace';
            app.ax_pdf.Position = [871 285 428 362];

            % Create ax_cdf
            app.ax_cdf = uiaxes(app.UIFigure);
            title(app.ax_cdf, 'Title')
            xlabel(app.ax_cdf, 'X')
            ylabel(app.ax_cdf, 'Y')
            zlabel(app.ax_cdf, 'Z')
            app.ax_cdf.NextPlot = 'replace';
            app.ax_cdf.Position = [871 285 428 362];

            % Create ax_pack
            app.ax_pack = uiaxes(app.UIFigure);
            title(app.ax_pack, 'Title')
            xlabel(app.ax_pack, 'X')
            ylabel(app.ax_pack, 'Y')
            zlabel(app.ax_pack, 'Z')
            app.ax_pack.NextPlot = 'replace';
            app.ax_pack.Position = [871 285 428 362];

            % Create Button_9
            app.Button_9 = uibutton(app.UIFigure, 'push');
            app.Button_9.Position = [45 106 277 161];
            app.Button_9.Text = '';

            % Create Button_8
            app.Button_8 = uibutton(app.UIFigure, 'state');
            app.Button_8.Tooltip = {'点击运行点击打包算法'};
            app.Button_8.Text = '';
            app.Button_8.Position = [43 421 278 138];

            % Create Button_7
            app.Button_7 = uibutton(app.UIFigure, 'state');
            app.Button_7.Text = '';
            app.Button_7.Position = [44 286 278 114];

            % Create Button_6
            app.Button_6 = uibutton(app.UIFigure, 'state');
            app.Button_6.Tooltip = {'点击运行点击打包算法'};
            app.Button_6.Text = '';
            app.Button_6.FontColor = [0.302 0.7451 0.9333];
            app.Button_6.Position = [55 466 255 56];

            % Create AODEditFieldLabel
            app.AODEditFieldLabel = uilabel(app.UIFigure);
            app.AODEditFieldLabel.HorizontalAlignment = 'right';
            app.AODEditFieldLabel.FontSize = 14;
            app.AODEditFieldLabel.Position = [56 343 91 22];
            app.AODEditFieldLabel.Text = 'AOD视场尺寸';

            % Create AODEditField
            app.AODEditField = uieditfield(app.UIFigure, 'numeric');
            app.AODEditField.ValueChangedFcn = createCallbackFcn(app, @AODEditFieldValueChanged, true);
            app.AODEditField.Position = [168 339 119 30];

            % Create import_button
            app.import_button = uibutton(app.UIFigure, 'push');
            app.import_button.ButtonPushedFcn = createCallbackFcn(app, @import_buttonPushed, true);
            app.import_button.FontSize = 14;
            app.import_button.Tooltip = {'点击导入数据'};
            app.import_button.Position = [46 581 101 40];
            app.import_button.Text = '导入数据';

            % Create Label
            app.Label = uilabel(app.UIFigure);
            app.Label.HorizontalAlignment = 'right';
            app.Label.FontSize = 14;
            app.Label.Position = [541 214 89 22];
            app.Label.Text = '运行结果展示';

            % Create Text_result
            app.Text_result = uitextarea(app.UIFigure);
            app.Text_result.FontSize = 14;
            app.Text_result.Position = [379 136 414 77];

            % Create EditFieldLabel
            app.EditFieldLabel = uilabel(app.UIFigure);
            app.EditFieldLabel.HorizontalAlignment = 'right';
            app.EditFieldLabel.FontSize = 14;
            app.EditFieldLabel.Position = [57 304 89 22];
            app.EditFieldLabel.Text = '最小转弯半径';

            % Create minradio
            app.minradio = uieditfield(app.UIFigure, 'numeric');
            app.minradio.ValueChangedFcn = createCallbackFcn(app, @minradioValueChanged, true);
            app.minradio.Position = [168 298 119 32];

            % Create Label_2
            app.Label_2 = uilabel(app.UIFigure);
            app.Label_2.FontSize = 14;
            app.Label_2.Position = [144 369 57 31];
            app.Label_2.Text = '参数调节';

            % Create GaeaxButton
            app.GaeaxButton = uibutton(app.UIFigure, 'push');
            app.GaeaxButton.ButtonPushedFcn = createCallbackFcn(app, @GaeaxButtonPushed, true);
            app.GaeaxButton.FontSize = 14;
            app.GaeaxButton.Tooltip = {'点击运行ga-eax算法'};
            app.GaeaxButton.Position = [77 174 104 31];
            app.GaeaxButton.Text = 'Ga-eax';

            % Create Packing
            app.Packing = uibutton(app.UIFigure, 'push');
            app.Packing.ButtonPushedFcn = createCallbackFcn(app, @PackingPushed, true);
            app.Packing.FontSize = 14;
            app.Packing.Tooltip = {'点击运行点击打包算法'};
            app.Packing.Position = [56 427 112 32];
            app.Packing.Text = '点集打包';

            % Create Label_3
            app.Label_3 = uilabel(app.UIFigure);
            app.Label_3.FontSize = 13;
            app.Label_3.Tooltip = {'点击运行点击打包算法'};
            app.Label_3.Position = [148 493 85 31];
            app.Label_3.Text = '点集线性放缩';

            % Create xEditFieldLabel
            app.xEditFieldLabel = uilabel(app.UIFigure);
            app.xEditFieldLabel.HorizontalAlignment = 'right';
            app.xEditFieldLabel.FontSize = 14;
            app.xEditFieldLabel.Tooltip = {'点击运行点击打包算法'};
            app.xEditFieldLabel.Position = [60 474 68 22];
            app.xEditFieldLabel.Text = 'x方向极值';

            % Create xEditField
            app.xEditField = uieditfield(app.UIFigure, 'numeric');
            app.xEditField.ValueChangedFcn = createCallbackFcn(app, @xEditFieldValueChanged, true);
            app.xEditField.Tooltip = {'点击运行点击打包算法'};
            app.xEditField.Position = [133 474 43 22];

            % Create yEditFieldLabel
            app.yEditFieldLabel = uilabel(app.UIFigure);
            app.yEditFieldLabel.HorizontalAlignment = 'right';
            app.yEditFieldLabel.FontSize = 14;
            app.yEditFieldLabel.Tooltip = {'点击运行点击打包算法'};
            app.yEditFieldLabel.Position = [181 474 68 22];
            app.yEditFieldLabel.Text = 'y方向极值';

            % Create yEditField
            app.yEditField = uieditfield(app.UIFigure, 'numeric');
            app.yEditField.ValueChangedFcn = createCallbackFcn(app, @yEditFieldValueChanged, true);
            app.yEditField.Tooltip = {'点击运行点击打包算法'};
            app.yEditField.Position = [257 474 46 22];

            % Create Button_2
            app.Button_2 = uibutton(app.UIFigure, 'push');
            app.Button_2.ButtonPushedFcn = createCallbackFcn(app, @Button_2Pushed, true);
            app.Button_2.FontSize = 14;
            app.Button_2.Tooltip = {'点击对打包效果进行可视化'};
            app.Button_2.Position = [197 427 113 32];
            app.Button_2.Text = '打包效果可视化';

            % Create DropDown
            app.DropDown = uidropdown(app.UIFigure);
            app.DropDown.Items = {'路径图', '打包算法效果图', '累计概率曲线', '概率密度曲线'};
            app.DropDown.ValueChangedFcn = createCallbackFcn(app, @DropDownValueChanged, true);
            app.DropDown.Tooltip = {'选择展示的图片'};
            app.DropDown.FontSize = 14;
            app.DropDown.Position = [378 589 156 33];
            app.DropDown.Value = '路径图';

            % Create THIOAButton
            app.THIOAButton = uibutton(app.UIFigure, 'push');
            app.THIOAButton.ButtonPushedFcn = createCallbackFcn(app, @THIOAButtonPushed, true);
            app.THIOAButton.FontSize = 14;
            app.THIOAButton.Tooltip = {'点击运行THIOA算法'};
            app.THIOAButton.Position = [77 223 104 31];
            app.THIOAButton.Text = 'THIOA';

            % Create Text_importdata
            app.Text_importdata = uitextarea(app.UIFigure);
            app.Text_importdata.FontSize = 13;
            app.Text_importdata.Position = [170 580 148 41];

            % Create Button_3
            app.Button_3 = uibutton(app.UIFigure, 'push');
            app.Button_3.ButtonPushedFcn = createCallbackFcn(app, @Button_3Pushed, true);
            app.Button_3.FontSize = 14;
            app.Button_3.Tooltip = {'点击绘制路径结果图'};
            app.Button_3.Position = [192 223 104 31];
            app.Button_3.Text = '结果图';

            % Create CDFButton
            app.CDFButton = uibutton(app.UIFigure, 'push');
            app.CDFButton.ButtonPushedFcn = createCallbackFcn(app, @CDFButtonPushed, true);
            app.CDFButton.FontSize = 14;
            app.CDFButton.Tooltip = {'点击绘制对数曲率累计概率曲线图'};
            app.CDFButton.Position = [192 174 104 31];
            app.CDFButton.Text = 'CDF图';

            % Create PDFButton
            app.PDFButton = uibutton(app.UIFigure, 'push');
            app.PDFButton.ButtonPushedFcn = createCallbackFcn(app, @PDFButtonPushed, true);
            app.PDFButton.FontSize = 14;
            app.PDFButton.Tooltip = {'点击绘制对数曲率概率密度曲线图'};
            app.PDFButton.Position = [192 124 104 31];
            app.PDFButton.Text = 'PDF图';

            % Create Label_4
            app.Label_4 = uilabel(app.UIFigure);
            app.Label_4.FontSize = 14;
            app.Label_4.Tooltip = {'点击运行点击打包算法'};
            app.Label_4.Position = [140 529 89 22];
            app.Label_4.Text = '点集打包算法';

            % Create resultTable
            app.resultTable = uitable(app.UIFigure);
            app.resultTable.ColumnName = {'文件名'; '点数'; '打包后点数'; '弃置点数'; 'THIOA'; 'Ga_eax'; 'A_D_GWO'};
            app.resultTable.RowName = {};
            app.resultTable.RowStriping = 'off';
            app.resultTable.Position = [45 12 738 80];

            % Create ADGWOButton
            app.ADGWOButton = uibutton(app.UIFigure, 'push');
            app.ADGWOButton.ButtonPushedFcn = createCallbackFcn(app, @ADGWOButtonPushed, true);
            app.ADGWOButton.FontSize = 14;
            app.ADGWOButton.Tooltip = {'点击运行A-D-GWO算法'};
            app.ADGWOButton.Position = [77 124 104 31];
            app.ADGWOButton.Text = 'A-D-GWO';

            % Create Label_5
            app.Label_5 = uilabel(app.UIFigure);
            app.Label_5.WordWrap = 'on';
            app.Label_5.FontSize = 14;
            app.Label_5.Position = [55 151 13 91];
            app.Label_5.Text = '算法选择';

            % Create Label_6
            app.Label_6 = uilabel(app.UIFigure);
            app.Label_6.WordWrap = 'on';
            app.Label_6.FontSize = 14;
            app.Label_6.Position = [302 158 17 76];
            app.Label_6.Text = '相关绘图';

            % Create excelButton
            app.excelButton = uibutton(app.UIFigure, 'state');
            app.excelButton.ValueChangedFcn = createCallbackFcn(app, @excelButtonValueChanged, true);
            app.excelButton.Tooltip = {'点击将当前表格导出成excel'};
            app.excelButton.Text = '导出成excel';
            app.excelButton.FontSize = 14;
            app.excelButton.Position = [398 95 167 28];

            % Create Button_10
            app.Button_10 = uibutton(app.UIFigure, 'state');
            app.Button_10.ValueChangedFcn = createCallbackFcn(app, @Button_10ValueChanged, true);
            app.Button_10.Tooltip = {'一键计算所有算法'};
            app.Button_10.Text = '一键计算并存储';
            app.Button_10.FontSize = 14;
            app.Button_10.Position = [582 94 167 28];

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = app1_exported

            % Create UIFigure and components
            createComponents(app)

            % Register the app with App Designer
            registerApp(app, app.UIFigure)

            % Execute the startup function
            runStartupFcn(app, @startupFcn)

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.UIFigure)
        end
    end
end