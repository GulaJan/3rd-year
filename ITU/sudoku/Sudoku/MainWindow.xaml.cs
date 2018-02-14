using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Threading;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
namespace Sudoku
{
    public partial class MainWindow
    {
      
        private SudokuViewModel sudoku;
        private SudokuLevel currentLevel;
        private TextBlock[,] sudokuTextBlocks;

        DispatcherTimer dt = new DispatcherTimer();
        Stopwatch stopWatch = new Stopwatch();
        string currentTime = string.Empty;
        public MainWindow()
        {
            InitializeComponent();
            GenerateSudokuGrid();
            dt.Tick += new EventHandler(dt_Tick);
            dt.Interval = new TimeSpan(0, 0, 0, 0, 1);
        }
            

            


        private void NewGame(object sender, EventArgs args)
        {
            
            sudoku = new SudokuViewModel();
            sudoku.GenerateSudoku(currentLevel);
            sudoku.Boardfill = new int?[9][];
            for (int i = 0; i< 9; i++)
            {
                sudoku.Boardfill[i] = new int?[9];
            }
            for (int i = 0; i < 9; i++) 
                for (int j = 0; j < 9; j++)
                {
                    sudoku.Boardfill[i][j] = 1;
                    if (sudoku.Board[i][j] == null)
                    {
                        sudoku.Boardfill[i][j] = null;
                    }
                    sudokuTextBlocks[i, j].SetValue(TextBlock.TextProperty, sudoku.Board[i][j].ToString());
                    sudokuTextBlocks[i, j].SetValue(TextBlock.BackgroundProperty, Brushes.White);
                }

                    // for (int i = 0; i < 9; i++)
                    //for (int j = 0; j < 9; j++)
                    //  sudokuTextBlocks[i, j].SetBinding(TextBlock.TextProperty, new Binding($"Board[{i}][{j}]"));
                    DataContext = sudoku;
            stopWatch.Reset();
            dt.Start();
            stopWatch.Start();
            Move.Text = sudoku.Moves.ToString();
        }

        private void DifficultySlider_OnValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            currentLevel = (SudokuLevel)e.NewValue;
            DifficultyTextBlock.Text = currentLevel.ToString().ToUpper();
        }

        private void GenerateSudokuGrid()
        {
            sudokuTextBlocks = new TextBlock[9, 9];
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                {

                    var border = new Border
                    {
                        BorderBrush = Brushes.Black,
                        BorderThickness = GetThickness(i, j, 0.1, 0.3)
                    };
                    sudokuTextBlocks[i, j] = new TextBlock
                    {
                        FontSize = 5,
                        VerticalAlignment = VerticalAlignment.Center,
                        HorizontalAlignment = HorizontalAlignment.Center
                    };
                    border.Child = sudokuTextBlocks[i, j];
                    Grid.SetRow(border, i);
                    Grid.SetColumn(border, j);
                    SudokuGrid.Children.Add(border);
                }
        }

        private Thickness GetThickness(int i, int j, double thin, double thick)
        {
            var top = i % 3 == 0 ? thick : thin;
            var bottom = i % 3 == 2 ? thick : thin;
            var left = j % 3 == 0 ? thick : thin;
            var right = j % 3 == 2 ? thick : thin;
            return new Thickness(left, top, right, bottom);
        }
    public int? showPad()
        {
            input InputPad;
            try
            {
                InputPad = new Sudoku.input();
                InputPad.Owner = this;
                System.Drawing.Point point = System.Windows.Forms.Control.MousePosition;
                InputPad.Left = this.Left + 440;
                InputPad.Top = this.Top + 250;
                InputPad.ShowDialog();
                return InputPad.state;
            }
            finally
            {
                InputPad = null;
            }




            }
        void dt_Tick(object sender, EventArgs e)
        {
            if (stopWatch.IsRunning)
            {
                TimeSpan ts = stopWatch.Elapsed;
                currentTime = String.Format("{0:00}:{1:00}:{2:00}",
                    ts.Hours, ts.Minutes, ts.Seconds);
                Timer.Text = currentTime;
            }
        }

        public void Mouse00(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[0][0] = pom;
                    sudokuTextBlocks[0, 0].SetValue(TextBlock.TextProperty, sudoku.Board[0][0].ToString());
                    if (!(rule(0, 0)))
                    {
                        sudokuTextBlocks[0, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
            
        }

        public void Mouse01(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    
                    sudoku.Board[0][1] = pom;
                    sudokuTextBlocks[0, 1].SetValue(TextBlock.TextProperty, sudoku.Board[0][1].ToString());
                    if (!(rule(0, 1)))
                    {
                        sudokuTextBlocks[0, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse02(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[0][2] = pom;
                    sudokuTextBlocks[0, 2].SetValue(TextBlock.TextProperty, sudoku.Board[0][2].ToString());
                    if (!(rule(0, 2)))
                    {
                        sudokuTextBlocks[0, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse03(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[0][3] = pom;
                    sudokuTextBlocks[0, 3].SetValue(TextBlock.TextProperty, sudoku.Board[0][3].ToString());
                    if (!(rule(0, 3)))
                    {
                        sudokuTextBlocks[0, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse04(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[0][4] = pom;
                    sudokuTextBlocks[0, 4].SetValue(TextBlock.TextProperty, sudoku.Board[0][4].ToString());
                    if (!(rule(0, 4)))
                    {
                        sudokuTextBlocks[0, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse05(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[0][5] = pom;
                    sudokuTextBlocks[0, 5].SetValue(TextBlock.TextProperty, sudoku.Board[0][5].ToString());
                    if (!(rule(0, 5)))
                    {
                        sudokuTextBlocks[0, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse06(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[0][6] = pom;
                    sudokuTextBlocks[0, 6].SetValue(TextBlock.TextProperty, sudoku.Board[0][6].ToString());
                    if (!(rule(0, 6)))
                    {
                        sudokuTextBlocks[0, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse07(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[0][7] = pom;
                    sudokuTextBlocks[0, 7].SetValue(TextBlock.TextProperty, sudoku.Board[0][7].ToString());
                    if (!(rule(0, 7)))
                    {
                        sudokuTextBlocks[0, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse08(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[0][8] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[0][8] = pom;
                    sudokuTextBlocks[0, 8].SetValue(TextBlock.TextProperty, sudoku.Board[0][8].ToString());
                    if (!(rule(0, 8)))
                    {
                        sudokuTextBlocks[0, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[0, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse10(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][0] = pom;
                    sudokuTextBlocks[1, 0].SetValue(TextBlock.TextProperty, sudoku.Board[1][0].ToString());
                    if (!(rule(1, 0)))
                    {
                        sudokuTextBlocks[1, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse11(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][1] = pom;
                    sudokuTextBlocks[1, 1].SetValue(TextBlock.TextProperty, sudoku.Board[1][1].ToString());
                    if (!(rule(1, 1)))
                    {
                        sudokuTextBlocks[1, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }

        }
        public void Mouse12(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][2] = pom;
                    sudokuTextBlocks[1, 2].SetValue(TextBlock.TextProperty, sudoku.Board[1][2].ToString());
                    if (!(rule(1, 2)))
                    {
                        sudokuTextBlocks[1, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse13(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][3] = pom;
                    sudokuTextBlocks[1, 3].SetValue(TextBlock.TextProperty, sudoku.Board[1][3].ToString());
                    if (!(rule(1, 3)))
                    {
                        sudokuTextBlocks[1, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse14(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][4] = pom;
                    sudokuTextBlocks[1, 4].SetValue(TextBlock.TextProperty, sudoku.Board[1][4].ToString());
                    if (!(rule(1, 4)))
                    {
                        sudokuTextBlocks[1, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse15(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][5] = pom;
                    sudokuTextBlocks[1, 5].SetValue(TextBlock.TextProperty, sudoku.Board[1][5].ToString());
                    if (!(rule(1, 5)))
                    {
                        sudokuTextBlocks[1, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse16(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][6] = pom;
                    sudokuTextBlocks[1, 6].SetValue(TextBlock.TextProperty, sudoku.Board[1][6].ToString());
                    if (!(rule(1, 6)))
                    {
                        sudokuTextBlocks[1, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse17(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][7] = pom;
                    sudokuTextBlocks[1, 7].SetValue(TextBlock.TextProperty, sudoku.Board[1][7].ToString());
                    if (!(rule(1, 7)))
                    {
                        sudokuTextBlocks[1, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse18(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[1][8] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[1][8] = pom;
                    sudokuTextBlocks[1, 8].SetValue(TextBlock.TextProperty, sudoku.Board[1][8].ToString());
                    if (!(rule(1, 8)))
                    {
                        sudokuTextBlocks[1, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[1, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse20(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][0] = pom;
                    sudokuTextBlocks[2, 0].SetValue(TextBlock.TextProperty, sudoku.Board[2][0].ToString());
                    if (!(rule(2, 0)))
                    {
                        sudokuTextBlocks[2, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse21(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][1] = pom;
                    sudokuTextBlocks[2, 1].SetValue(TextBlock.TextProperty, sudoku.Board[2][1].ToString());
                    if (!(rule(2, 1)))
                    {
                        sudokuTextBlocks[2, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse22(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][2] = pom;
                    sudokuTextBlocks[2, 2].SetValue(TextBlock.TextProperty, sudoku.Board[2][2].ToString());
                    if (!(rule(2, 2)))
                    {
                        sudokuTextBlocks[2, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse23(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][3] = pom;
                    sudokuTextBlocks[2, 3].SetValue(TextBlock.TextProperty, sudoku.Board[2][3].ToString());
                    if (!(rule(2, 3)))
                    {
                        sudokuTextBlocks[2, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse24(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][4] = pom;
                    sudokuTextBlocks[2, 4].SetValue(TextBlock.TextProperty, sudoku.Board[2][4].ToString());
                    if (!(rule(2, 4)))
                    {
                        sudokuTextBlocks[2, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse25(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][5] = pom;
                    sudokuTextBlocks[2, 5].SetValue(TextBlock.TextProperty, sudoku.Board[2][5].ToString());
                    if (!(rule(2, 5)))
                    {
                        sudokuTextBlocks[2, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse26(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][6] = pom;
                    sudokuTextBlocks[2, 6].SetValue(TextBlock.TextProperty, sudoku.Board[2][6].ToString());
                    if (!(rule(2, 6)))
                    {
                        sudokuTextBlocks[2, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse27(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][7] = pom;
                    sudokuTextBlocks[2, 7].SetValue(TextBlock.TextProperty, sudoku.Board[2][7].ToString());
                    if (!(rule(2, 7)))
                    {
                        sudokuTextBlocks[2, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse28(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[2][8] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[2][8] = pom;
                    sudokuTextBlocks[2, 8].SetValue(TextBlock.TextProperty, sudoku.Board[2][8].ToString());
                    if (!(rule(2, 8)))
                    {
                        sudokuTextBlocks[2, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[2, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse30(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][0] = pom;
                    sudokuTextBlocks[3, 0].SetValue(TextBlock.TextProperty, sudoku.Board[3][0].ToString());
                    if (!(rule(3, 0)))
                    {
                        sudokuTextBlocks[3, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
      
        public void Mouse31(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][1] = pom;
                    sudokuTextBlocks[3, 1].SetValue(TextBlock.TextProperty, sudoku.Board[3][1].ToString());
                    if (!(rule(3, 1)))
                    {
                        sudokuTextBlocks[3, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse32(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][2] = pom;
                    sudokuTextBlocks[3, 2].SetValue(TextBlock.TextProperty, sudoku.Board[3][2].ToString());
                    if (!(rule(3, 2)))
                    {
                        sudokuTextBlocks[3, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse33(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][3] = pom;
                    sudokuTextBlocks[3, 3].SetValue(TextBlock.TextProperty, sudoku.Board[3][3].ToString());
                    if (!(rule(3, 3)))
                    {
                        sudokuTextBlocks[3, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse34(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][4] = pom;
                    sudokuTextBlocks[3, 4].SetValue(TextBlock.TextProperty, sudoku.Board[3][4].ToString());
                    if (!(rule(3, 4)))
                    {
                        sudokuTextBlocks[3, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse35(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][5] = pom;
                    sudokuTextBlocks[3, 5].SetValue(TextBlock.TextProperty, sudoku.Board[3][5].ToString());
                    if (!(rule(3, 5)))
                    {
                        sudokuTextBlocks[3, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse36(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][6] = pom;
                    sudokuTextBlocks[3, 6].SetValue(TextBlock.TextProperty, sudoku.Board[3][6].ToString());
                    if (!(rule(3, 6)))
                    {
                        sudokuTextBlocks[3, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse37(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][7] = pom;
                    sudokuTextBlocks[3, 7].SetValue(TextBlock.TextProperty, sudoku.Board[3][7].ToString());
                    if (!(rule(3, 7)))
                    {
                        sudokuTextBlocks[3, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse38(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[3][8] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[3][8] = pom;
                    sudokuTextBlocks[3, 8].SetValue(TextBlock.TextProperty, sudoku.Board[3][8].ToString());
                    if (!(rule(3, 8)))
                    {
                        sudokuTextBlocks[3, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[3, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse40(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][0] = pom;
                    sudokuTextBlocks[4, 0].SetValue(TextBlock.TextProperty, sudoku.Board[4][0].ToString());
                    if (!(rule(4, 0)))
                    {
                        sudokuTextBlocks[4, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse41(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][1] = pom;
                    sudokuTextBlocks[4, 1].SetValue(TextBlock.TextProperty, sudoku.Board[4][1].ToString());
                    if (!(rule(4, 1)))
                    {
                        sudokuTextBlocks[4, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse42(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][2] = pom;
                    sudokuTextBlocks[4, 2].SetValue(TextBlock.TextProperty, sudoku.Board[4][2].ToString());
                    if (!(rule(4, 2)))
                    {
                        sudokuTextBlocks[4, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse43(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][3] = pom;
                    sudokuTextBlocks[4, 3].SetValue(TextBlock.TextProperty, sudoku.Board[4][3].ToString());
                    if (!(rule(4, 3)))
                    {
                        sudokuTextBlocks[4, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse44(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][4] = pom;
                    sudokuTextBlocks[4, 4].SetValue(TextBlock.TextProperty, sudoku.Board[4][4].ToString());
                    if (!(rule(4, 4)))
                    {
                        sudokuTextBlocks[4, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse45(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][5] = pom;
                    sudokuTextBlocks[4, 5].SetValue(TextBlock.TextProperty, sudoku.Board[4][5].ToString());
                    if (!(rule(4, 5)))
                    {
                        sudokuTextBlocks[4, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse46(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][6] = pom;
                    sudokuTextBlocks[4, 6].SetValue(TextBlock.TextProperty, sudoku.Board[4][6].ToString());
                    if (!(rule(4, 6)))
                    {
                        sudokuTextBlocks[4, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse47(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][7] = pom;
                    sudokuTextBlocks[4, 7].SetValue(TextBlock.TextProperty, sudoku.Board[4][7].ToString());
                    if (!(rule(4, 7)))
                    {
                        sudokuTextBlocks[4, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse48(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[4][8] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[4][8] = pom;
                    sudokuTextBlocks[4, 8].SetValue(TextBlock.TextProperty, sudoku.Board[4][8].ToString());
                    if (!(rule(4, 8)))
                    {
                        sudokuTextBlocks[4, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[4, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse50(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][0] = pom;
                    sudokuTextBlocks[5, 0].SetValue(TextBlock.TextProperty, sudoku.Board[5][0].ToString());
                    if (!(rule(5, 0)))
                    {
                        sudokuTextBlocks[5, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse51(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][1] = pom;
                    sudokuTextBlocks[5, 1].SetValue(TextBlock.TextProperty, sudoku.Board[5][1].ToString());
                    if (!(rule(5, 1)))
                    {
                        sudokuTextBlocks[5, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse52(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][2] = pom;
                    sudokuTextBlocks[5, 2].SetValue(TextBlock.TextProperty, sudoku.Board[5][2].ToString());
                    if (!(rule(5, 2)))
                    {
                        sudokuTextBlocks[5, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse53(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][3] = pom;
                    sudokuTextBlocks[5, 3].SetValue(TextBlock.TextProperty, sudoku.Board[5][3].ToString());
                    if (!(rule(5, 3)))
                    {
                        sudokuTextBlocks[5, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse54(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][4] = pom;
                    sudokuTextBlocks[5, 4].SetValue(TextBlock.TextProperty, sudoku.Board[5][4].ToString());
                    if (!(rule(5, 4)))
                    {
                        sudokuTextBlocks[5, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse55(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][5] = pom;
                    sudokuTextBlocks[5, 5].SetValue(TextBlock.TextProperty, sudoku.Board[5][5].ToString());
                    if (!(rule(5, 5)))
                    {
                        sudokuTextBlocks[5, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse56(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][6] = pom;
                    sudokuTextBlocks[5, 6].SetValue(TextBlock.TextProperty, sudoku.Board[5][6].ToString());
                    if (!(rule(5, 6)))
                    {
                        sudokuTextBlocks[5, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse57(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][7] = pom;
                    sudokuTextBlocks[5, 7].SetValue(TextBlock.TextProperty, sudoku.Board[5][7].ToString());
                    if (!(rule(5, 7)))
                    {
                        sudokuTextBlocks[5, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse58(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[5][8] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[5][8] = pom;
                    sudokuTextBlocks[5, 8].SetValue(TextBlock.TextProperty, sudoku.Board[5][8].ToString());
                    if (!(rule(5, 8)))
                    {
                        sudokuTextBlocks[5, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[5, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse60(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][0] = pom;
                    sudokuTextBlocks[6, 0].SetValue(TextBlock.TextProperty, sudoku.Board[6][0].ToString());
                    if (!(rule(6, 0)))
                    {
                        sudokuTextBlocks[6, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse61(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][1] = pom;
                    sudokuTextBlocks[6, 1].SetValue(TextBlock.TextProperty, sudoku.Board[6][1].ToString());
                    if (!(rule(6, 1)))
                    {
                        sudokuTextBlocks[6, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse62(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][2] = pom;
                    sudokuTextBlocks[6, 2].SetValue(TextBlock.TextProperty, sudoku.Board[6][2].ToString());
                    if (!(rule(6, 2)))
                    {
                        sudokuTextBlocks[6, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse63(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][3] = pom;
                    sudokuTextBlocks[6, 3].SetValue(TextBlock.TextProperty, sudoku.Board[6][3].ToString());
                    if (!(rule(6, 3)))
                    {
                        sudokuTextBlocks[6, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse64(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][4] = pom;
                    sudokuTextBlocks[6, 4].SetValue(TextBlock.TextProperty, sudoku.Board[6][4].ToString());
                    if (!(rule(6, 4)))
                    {
                        sudokuTextBlocks[6, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse65(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][5] = pom;
                    sudokuTextBlocks[6, 5].SetValue(TextBlock.TextProperty, sudoku.Board[6][5].ToString());
                    if (!(rule(6, 5)))
                    {
                        sudokuTextBlocks[6, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse66(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][6] = pom;
                    sudokuTextBlocks[6, 6].SetValue(TextBlock.TextProperty, sudoku.Board[6][6].ToString());
                    if (!(rule(6, 6)))
                    {
                        sudokuTextBlocks[6, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse67(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][7] = pom;
                    sudokuTextBlocks[6, 7].SetValue(TextBlock.TextProperty, sudoku.Board[6][7].ToString());
                    if (!(rule(6, 7)))
                    {
                        sudokuTextBlocks[6, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse68(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[6][8] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[6][8] = pom;
                    sudokuTextBlocks[6, 8].SetValue(TextBlock.TextProperty, sudoku.Board[6][8].ToString());
                    if (!(rule(6, 8)))
                    {
                        sudokuTextBlocks[6, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[6, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse70(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][0] = pom;
                    sudokuTextBlocks[7, 0].SetValue(TextBlock.TextProperty, sudoku.Board[7][0].ToString());
                    if (!(rule(7, 0)))
                    {
                        sudokuTextBlocks[7, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse71(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][1] = pom;
                    sudokuTextBlocks[7, 1].SetValue(TextBlock.TextProperty, sudoku.Board[7][1].ToString());
                    if (!(rule(7, 1)))
                    {
                        sudokuTextBlocks[7, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
            
        }

        public void Mouse72(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][2] = pom;
                    sudokuTextBlocks[7, 2].SetValue(TextBlock.TextProperty, sudoku.Board[7][2].ToString());
                    if (!(rule(7, 2)))
                    {
                        sudokuTextBlocks[7, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse73(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][3] = pom;
                    sudokuTextBlocks[7, 3].SetValue(TextBlock.TextProperty, sudoku.Board[7][3].ToString());
                    if (!(rule(7, 3)))
                    {
                        sudokuTextBlocks[7, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse74(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][4] = pom;
                    sudokuTextBlocks[7, 4].SetValue(TextBlock.TextProperty, sudoku.Board[7][4].ToString());
                    if (!(rule(7, 4)))
                    {
                        sudokuTextBlocks[7, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse75(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][5] = pom;
                    sudokuTextBlocks[7, 5].SetValue(TextBlock.TextProperty, sudoku.Board[7][5].ToString());
                    if (!(rule(7, 5)))
                    {
                        sudokuTextBlocks[7, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse76(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][6] = pom;
                    sudokuTextBlocks[7, 6].SetValue(TextBlock.TextProperty, sudoku.Board[7][6].ToString());
                    if (!(rule(7, 6)))
                    {
                        sudokuTextBlocks[7, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse77(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][7] = pom;
                    sudokuTextBlocks[7, 7].SetValue(TextBlock.TextProperty, sudoku.Board[7][7].ToString());
                    if (!(rule(7, 7)))
                    {
                        sudokuTextBlocks[7, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse78(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[7][8] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[7][8] = pom;
                    sudokuTextBlocks[7, 8].SetValue(TextBlock.TextProperty, sudoku.Board[7][8].ToString());
                    if (!(rule(7, 8)))
                    {
                        sudokuTextBlocks[7, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[7, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse80(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][0] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][0] = pom;
                    sudokuTextBlocks[8, 0].SetValue(TextBlock.TextProperty, sudoku.Board[8][0].ToString());
                    if (!(rule(8, 0)))
                    {
                        sudokuTextBlocks[8, 0].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 0].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse81(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][1] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][1] = pom;
                    sudokuTextBlocks[8, 1].SetValue(TextBlock.TextProperty, sudoku.Board[8][1].ToString());
                    if (!(rule(8, 1)))
                    {
                        sudokuTextBlocks[8, 1].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 1].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public void Mouse82(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][2] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][2] = pom;
                    sudokuTextBlocks[8, 2].SetValue(TextBlock.TextProperty, sudoku.Board[8][2].ToString());
                    if (!(rule(8, 2)))
                    {
                        sudokuTextBlocks[8, 2].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 2].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse83(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][3] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][3] = pom;
                    sudokuTextBlocks[8, 3].SetValue(TextBlock.TextProperty, sudoku.Board[8][3].ToString());
                    if (!(rule(8, 3)))
                    {
                        sudokuTextBlocks[8, 3].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 3].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse84(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][4] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][4] = pom;
                    sudokuTextBlocks[8, 4].SetValue(TextBlock.TextProperty, sudoku.Board[8][4].ToString());
                    if (!(rule(8, 4)))
                    {
                        sudokuTextBlocks[8, 4].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 4].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse85(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][5] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][5] = pom;
                    sudokuTextBlocks[8, 5].SetValue(TextBlock.TextProperty, sudoku.Board[8][5].ToString());
                    if (!(rule(8, 5)))
                    {
                        sudokuTextBlocks[8, 5].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 5].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();

            }
        }
        public void Mouse86(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][6] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][6] = pom;
                    sudokuTextBlocks[8, 6].SetValue(TextBlock.TextProperty, sudoku.Board[8][6].ToString());
                    if (!(rule(8, 6)))
                    {
                        sudokuTextBlocks[8, 6].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 6].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }
        public void Mouse87(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][7] == null)
            {
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][7] = pom;
                    sudokuTextBlocks[8, 7].SetValue(TextBlock.TextProperty, sudoku.Board[8][7].ToString());

                    if (!(rule(8, 7)))
                    {
                        sudokuTextBlocks[8, 7].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 7].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                    
                }
                check_full();
            }
        }
        public void Mouse88(object sender, RoutedEventArgs e)
        {
            if (sudoku.Boardfill[8][8] == null)
            {
              
                int? pom = showPad();
                if (!(pom == null))
                {
                    sudoku.Board[8][8] = pom;
                    sudokuTextBlocks[8, 8].SetValue(TextBlock.TextProperty, sudoku.Board[8][8].ToString());

                    if (!(rule(8, 8)))
                    {
                        sudokuTextBlocks[8, 8].SetValue(TextBlock.BackgroundProperty, Brushes.Red);
                    }
                    else
                        sudokuTextBlocks[8, 8].SetValue(TextBlock.BackgroundProperty, Brushes.DarkGray);
                }
                check_full();
            }
        }

        public bool rule(int i, int j)
        {
            sudoku.moves++;
            Move.Text = sudoku.Moves.ToString();
            switch (i)
            {
                case 0:
                    if (!(rule_row(i, j, 1, 9)))
                        return false;
                    break;
                case 1:
                    if (!(rule_row(i, j, 0, 1)))
                        return false;
                    if (!(rule_row(i, j, 2, 9)))
                        return false;
                    break;
                case 2:
                    if (!(rule_row(i, j, 0, 2)))
                        return false;
                    if (!(rule_row(i, j, 3, 9)))
                        return false;
                    break;
                case 3:
                    if (!(rule_row(i, j, 0, 3)))
                        return false;
                    if (!(rule_row(i, j, 4, 9)))
                        return false;
                    break;
                case 4:
                    if (!(rule_row(i, j, 0, 4)))
                        return false;
                    if (!(rule_row(i, j, 5, 9)))
                        return false;
                    break;
                case 5:
                    if (!(rule_row(i, j, 0, 5)))
                        return false;
                    if (!(rule_row(i, j, 6, 9)))
                        return false;
                    break;
                case 6:
                    if (!(rule_row(i, j, 0, 6)))
                        return false;
                    if (!(rule_row(i, j, 7, 9)))
                        return false;
                    break;
                case 7:
                    if (!(rule_row(i, j, 0, 7)))
                        return false;
                    if (!(rule_row(i, j, 8, 9)))
                        return false;
                    break;
                case 8:
                    if (!(rule_row(i, j, 0, 8)))
                        return false;
                    break;
                default:
                    break;

            }

            switch (j)
            {
                case 0:
                    if (!(rule_column(i, j, 1, 9)))
                        return false;
                    break;
                case 1:
                    if (!(rule_column(i, j, 0, 1)))
                        return false;
                    if (!(rule_column(i, j, 2, 9)))
                        return false;
                    break;
                case 2:
                    if (!(rule_column(i, j, 0, 2)))
                        return false;
                    if (!(rule_column(i, j, 3, 9)))
                        return false;
                    break;
                case 3:
                    if (!(rule_column(i, j, 0, 3)))
                        return false;
                    if (!(rule_column(i, j, 4, 9)))
                        return false;
                    break;
                case 4:
                    if (!(rule_column(i, j, 0, 4)))
                        return false;
                    if (!(rule_column(i, j, 5, 9)))
                        return false;
                    break;
                case 5:
                    if (!(rule_column(i, j, 0, 5)))
                        return false;
                    if (!(rule_column(i, j, 6, 9)))
                        return false;
                    break;
                case 6:
                    if (!(rule_column(i, j, 0, 6)))
                        return false;
                    if (!(rule_column(i, j, 7, 9)))
                        return false;
                    break;
                case 7:
                    if (!(rule_column(i, j, 0, 7)))
                        return false;
                    if (!(rule_column(i, j, 8, 9)))
                        return false;
                    break;
                case 8:
                    if (!(rule_column(i, j, 0, 8)))
                        return false;
                    break;
                default:
                    break;
            }
            if (i == 0 || i == 3 || i == 6)
            {
                if (j == 0 || j == 3 || j == 6)
                {
                    if (!(rule_square(i, j, i + 1, j + 1)) || !(rule_square(i, j, i + 1, j + 2)) || !(rule_square(i, j, i + 2, j + 1)) || !(rule_square(i, j, i + 2, j + 2)))
                        return false;
                }

                if (j == 1 || j == 4 || j == 7)
                {
                    if (!(rule_square(i, j, i + 1, j - 1)) || !(rule_square(i, j, i + 1, j + 1)) || !(rule_square(i, j, i + 2, j - 1)) || !(rule_square(i, j, i + 2, j + 1)))
                        return false;
                }

                if (j == 2 || j == 5|| j == 8)
                {
                    if (!(rule_square(i, j, i + 1, j - 1)) || !(rule_square(i, j, i + 1, j - 2)) || !(rule_square(i, j, i + 2, j - 1)) || !(rule_square(i, j, i + 2, j - 2)))
                        return false;
                }
            }

            if (i == 1 || i == 4 || i == 7)
            {
                if (j == 0 || j == 3 || j == 6)
                {
                    if (!(rule_square(i, j, i - 1, j + 1)) || !(rule_square(i, j, i - 1, j + 2)) || !(rule_square(i, j, i + 1, j + 1)) || !(rule_square(i, j, i + 1, j + 2)))
                        return false;
                }

                if (j == 1 || j == 4 || j == 7)
                {
                    if (!(rule_square(i, j, i - 1, j - 1)) || !(rule_square(i, j, i - 1, j + 1)) || !(rule_square(i, j, i + 1, j - 1)) || !(rule_square(i, j, i + 1, j + 1)))
                        return false;
                }

                if (j == 2 || j == 5 || j == 8)
                {
                    if (!(rule_square(i, j, i - 1, j - 1)) || !(rule_square(i, j, i - 1, j - 2)) || !(rule_square(i, j, i + 1, j - 1)) || !(rule_square(i, j, i + 1, j - 2)))
                        return false;
                }
            }

            if (i == 2 || i == 5 || i == 8)
            {
                if (j == 0 || j == 3 || j == 6)
                {
                    if (!(rule_square(i, j, i - 1, j + 1)) || !(rule_square(i, j, i - 1, j + 2)) || !(rule_square(i, j, i - 2, j + 1)) || !(rule_square(i, j, i - 2, j + 2)))
                        return false;
                }

                if (j == 1 || j == 4 || j == 7)
                {
                    if (!(rule_square(i, j, i - 1, j - 1)) || !(rule_square(i, j, i - 1, j + 1)) || !(rule_square(i, j, i - 2, j - 1)) || !(rule_square(i, j, i - 2, j + 1)))
                        return false;
                }

                if (j == 2 || j == 5 || j == 8)
                {
                    if (!(rule_square(i, j, i - 1, j - 1)) || !(rule_square(i, j, i - 1, j - 2)) || !(rule_square(i, j, i - 2, j - 1)) || !(rule_square(i, j, i - 2, j - 2)))
                        return false;
                }
            }
            return true;
        }

        public bool rule_row(int i, int j, int n, int m)
        {
            for (int l = n; l < m; l++)
            {
                if (sudoku.Board[i][j] == sudoku.Board[l][j])
                {
                    return false;
                }
     
            }
            return true;
        }

        public bool rule_column(int i, int j, int n, int m)
        {
            for (int l = n; l < m; l++)
            {
                if (sudoku.Board[i][j] == sudoku.Board[i][l])
                {
                    return false;
                }

            }
            return true;
        }

        public bool rule_square(int i, int j, int n, int m)
        {
            if (sudoku.Board[i][j] == sudoku.Board[n][m])
            {
                return false;
            }
            else
                return true;
                
        }
        
        public void check_full()
        {
            int full = 1;
            for (int i = 0; i < 9; i++)
                for (int j = 0; j< 9; j++)
                {
                    if ((sudoku.Board [i][j] == null) || (sudokuTextBlocks[i, j].GetValue(TextBlock.BackgroundProperty)) == Brushes.Red)
                    {
                        full = 0;
                        break;
                    }
                    if (full == 0)
                        break;
                }
                if (full == 1)
            {
                stopWatch.Stop();
                string fileName = @"score.txt";
                using (FileStream fs = new FileStream(fileName, FileMode.Append, FileAccess.Write))
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    sw.WriteLine(currentTime + " počet ťahov: " + sudoku.Moves.ToString());
                }
                MessageBox.Show("Gratulujeme vam, uspesne ste dohrali a vas cas je " + currentTime);
                stopWatch.Reset();
            }
        }

        private void Score_Click(object sender, RoutedEventArgs e)
        {
            Tabulka tabulka;
            if (File.Exists("score.txt"))
            {
                string[] TopScores = File.ReadAllLines("score.txt");
                var OrderScore = TopScores.OrderBy(x => DateTime.Parse(x.Split(' ')[0]));
                //string.Join( Environment.NewLine, OrderScore);
                tabulka = new Sudoku.Tabulka();
                tabulka.Owner = this;
                string text = string.Empty;
                int i = 1;
                foreach (string line in OrderScore)
                {
                    text = text + i + ".miesto:     " + line + '\n';
                    i++;
                }
                tabulka.textBlock.Text = text;
            }
            else
            {
                tabulka = new Sudoku.Tabulka();
                tabulka.Owner = this;
            }
            //tabulka.textBlock.Text = string.Join(Environment.NewLine, OrderScore);
            
            tabulka.ShowDialog();
            


        }
    }
}
