package Chart;

import CSV.ReaderCSVtoArray_Funktions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import static CSV.ReaderCSVtoArray_Funktions.FindeNumberOfRowCsv;
import static gui.Windo.*;

public class plotTheFirstChannle_Button implements ActionListener {
    public plotTheFirstChannle_Button() {
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        try {
            plot();
        } catch (IOException ioException) {
            ioException.printStackTrace();
        }
    }

    void plot() throws IOException {





        for (int i = 1; i< FindeNumberOfRowCsv(); i++ )
        {
            try {
                series1.add(i++, Double.parseDouble(ReaderCSVtoArray_Funktions.PlotForFirstChart[i]));

            }catch (Exception NumberFormatException){


            }



        }

        dataset.addSeries(series1);




    }
}