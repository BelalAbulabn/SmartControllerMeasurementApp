package Chart;

import CSV.ReaderCSVtoArray_Funktions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import static CSV.ReaderCSVtoArray_Funktions.FindeNumberOfRowCsv;
import static gui.Windo.dataset;
import static gui.Windo.series3;

public class plotTheThirdChannle_Button implements ActionListener {
    public plotTheThirdChannle_Button() {
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
                series3.add(i++, Double.parseDouble(ReaderCSVtoArray_Funktions.PlotForThirdChart[i]));

            }catch (Exception NumberFormatException){


            }



        }

        dataset.addSeries(series3);




    }
}