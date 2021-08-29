import java.io.*;

public class FileAnalyser {

    String inputFileName, outputFileName;
    StringBuilder input, output;
    int[] data;

    FileReader inputHandler;
    FileWriter outputHandler;

    FileAnalyser(String inputFileName, String outputFileName) {
        this.inputFileName = inputFileName;
        this.outputFileName = outputFileName;
        input = new StringBuilder();
        output = new StringBuilder();
        data = new int[65536];
    }

    public void run() throws Exception {
        openInputFile();
        readInputFile();
        analizeData();
        createOutputData();
        openOutputFile();
        writeOutputFile();
    }

    private void openInputFile() throws Exception {
        File handler = new File(inputFileName);
        if (!handler.exists())
            throw new Exception("Input file not found");
        if (!handler.canRead())
            throw new Exception("No read access permission");
        inputHandler = new FileReader(handler);
    }

    private void openOutputFile() throws Exception {
        File handler = new File(outputFileName);
        if (!handler.exists())
            handler.createNewFile();
        if (!handler.canWrite())
            throw new Exception("No write access permission");
        outputHandler = new FileWriter(handler);
    }

    private void readInputFile() throws IOException {
        BufferedReader reader = new BufferedReader(inputHandler);
        String line = reader.readLine();
        while (line != null) {
            input.append(line);
            line = reader.readLine();
        }
        reader.close();
        inputHandler.close();
    }

    private void writeOutputFile() throws IOException {
        BufferedWriter writer = new BufferedWriter(outputHandler);
        writer.write(output.toString());
        writer.close();
        outputHandler.close();
    }

    private void analizeData() {
        for (int position = 0; position < input.length(); position++) {
            int char_code = (int) input.charAt(position);
            data[char_code] += 1;
        }
    }

    private void createOutputData() {
        output.append("File: " + inputFileName + "\n");
        output.append("Total count of chars: " + input.length() + "\n\n");
        for (int position = 0; position < 65536; position++) {
            if (data[position] == 0)
                continue;
            if ((char) position == '\u0020') {
                output.append("whitespace" + " : " + data[position] + "\n");
                continue;
            }
            char symbol = (char) position;
            output.append(symbol + " : " + data[position] + "\n");
        }
    }
}
