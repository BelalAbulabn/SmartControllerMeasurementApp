package Chart;

public class ChartPlot {
    private String[] CsvContent;
    private int  NumberOfLines;

    public ChartPlot() {
    }

    public String[] getCsvContent() {
        return CsvContent;
    }

    public void setCsvContent(String[] csvContent) {

        CsvContent = csvContent;
    }

    public int getNumberOfLines() {
        return NumberOfLines;
    }

    public void setNumberOfLines(int numberOfLines) {
        NumberOfLines = numberOfLines;
    }
}
