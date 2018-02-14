
using System.Windows;


namespace Sudoku
{
    /// <summary>
    /// Interaction logic for input.xaml
    /// </summary>
    public partial class input : Window
    {
        public int? state = null;
        public input()
        {
            InitializeComponent();
        }

        public void Button1(object sender, RoutedEventArgs e)
        {
            state = 1;
            this.Close();
        }

        public void Button2(object sender, RoutedEventArgs e)
        {
            state = 2;
            this.Close();
        }

        public void Button3(object sender, RoutedEventArgs e)
        {
            state = 3;
            this.Close();
        }

        public void Button4(object sender, RoutedEventArgs e)
        {
            state = 4;
            this.Close();
        }

        public void Button5(object sender, RoutedEventArgs e)
        {
            state = 5;
            this.Close();
        }

        public void Button6(object sender, RoutedEventArgs e)
        {
            state = 6;
            this.Close();
        }

        public void Button7(object sender, RoutedEventArgs e)
        {
            state = 7;
            this.Close();
        }

        public void Button8(object sender, RoutedEventArgs e)
        {
            state = 8;
            this.Close();
        }

        public void Button9 (object sender, RoutedEventArgs e)
        {
            state = 9;
            this.Close();
        }



    }
}
