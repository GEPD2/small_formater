import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;


public class SimpleInstaller extends JFrame {
    private JLabel statusBar;       // Label to display status messages at the bottom
    private JButton installButton;  // Button to start installation process

    public SimpleInstaller() {
        setTitle("Simple Installer");              // Window title
        setSize(400, 150);                         // Window size
        setDefaultCloseOperation(EXIT_ON_CLOSE);  // Close app when window closes
        setLayout(new BorderLayout());             // Use BorderLayout for layout

        // Creating and adding a welcome label at the top (NORTH)
        JLabel welcomeLabel = new JLabel("Welcome to the Installer!", SwingConstants.CENTER);
        welcomeLabel.setFont(new Font("SansSerif", Font.BOLD, 16));  // Set font style and size
        add(welcomeLabel, BorderLayout.NORTH);

        // Creation of the status bar label at the bottom (SOUTH) with padding
        statusBar = new JLabel("Ready");
        statusBar.setBorder(BorderFactory.createEmptyBorder(5, 10, 5, 10));
        add(statusBar, BorderLayout.SOUTH);

        // Creation of the install button and add it to a centered panel
        installButton = new JButton("Start Installation");
        installButton.addActionListener(new InstallAction()); // Set what happens on click

        JPanel centerPanel = new JPanel();  // A simple panel to hold the button centered
        centerPanel.add(installButton);
        add(centerPanel, BorderLayout.CENTER);

        // Show detected OS in the status bar at startup
        String os = getOS();
        statusBar.setText("Detected OS: " + os);
    }

    // Static method to detect the running operating system
    public static String getOS() {
        String osName = System.getProperty("os.name").toLowerCase();

        if (osName.contains("win")) {
            return "Windows";
        } else if (osName.contains("mac")) {
            return "MacOS";
        } else if (osName.contains("nix") || osName.contains("nux") || osName.contains("aix")) {
            return "Linux";
        } else if (osName.contains("android")) {
            return "Android";
        } else if (osName.contains("ios")) {
            return "iOS";
        } else {
            return "Unknown";
        }
    }

    // Inner class handles installation button click events
    private class InstallAction implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            installButton.setEnabled(false);    // Disable button to avoid multiple clicks
            statusBar.setText("Installation started...");  // Update status bar
            // getting running os
            String os = getOS();

            // Running installation steps on a new thread so UI remains responsive
            new Thread(() -> {
                ProcessBuilder pb;
                Process process;
                int exitCode,status_all=0;
                if(os== "iOS"){
                    updateStatus("iOs is not in the list yet");
                }
                else if(os=="Unknown")
                {
                    updateStatus("unknown system is not in the list yet");
                }
                else{
                    try {
                        // steps to build the application
                        updateStatus("Preparing installation...");
                        Thread.sleep(1000);
                        updateStatus("The screen will flash,don't touch it");
                        Thread.sleep(1000);
                        updateStatus("If sudo password is required give it to install the requirements");
                        Thread.sleep(1000);
                        //checking qmake
                        try {
                            pb = new ProcessBuilder("qmake", "-v");
                            process = pb.start();  // <-- IOException might happen here
                            exitCode = process.waitFor();
                            if (exitCode == 0) {
                                updateStatus("qmake is installed.");
                                status_all=1;
                            } else {
                                updateStatus("qmake is not installed.");
                                try{
                                    int android_status=0;
                                    if(os == "Linux"){
                                        pb = new ProcessBuilder("xterm", "-e", "sudo apt install qtbase5-dev qtcreator -y");
                                    }
                                    else if(os == "Windows"){
                                        pb = new ProcessBuilder("powershell", "-Command", "Start-Process powershell -Verb runAs -ArgumentList 'winget install -e --id QtProject.QtCreator'");
                                    }
                                    else if(os == "MacOS"){
                                        pb = new ProcessBuilder("osascript", "-e", "do shell script \"brew install --cask qt-creator\" with administrator privileges");
                                    }
                                    else if(os == "Android")
                                    {
                                        android_status=1;
                                        pb = new ProcessBuilder("su", "-c", "apt install qmake -y");  // Example inside Termux
                                    }
                                    process = pb.start();
                                    exitCode = process.waitFor();
                                    if(android_status == 1 && exitCode == 0){
                                        updateStatus("installation finished");
                                        status_all=1;
                                    }
                                    else if(android_status == 1 && exitCode != 0){
                                        updateStatus("trying no root method");
                                        pb = new ProcessBuilder("bash","-c","apt install qmake -y");
                                        process = pb.start();
                                        exitCode = process.waitFor();
                                        if(exitCode == 0){
                                            updateStatus("installation finished");
                                            status_all=1;
                                        }
                                        else{
                                            updateStatus("installation failed");
                                        }
                                    }
                                    else if(android_status == 0 && exitCode == 0)
                                    {
                                        updateStatus("installation finished");
                                        status_all=1;
                                    }
                                    else if(android_status == 0 && exitCode != 0){
                                        updateStatus("installation failed");
                                    }
                                }
                                catch (IOException | InterruptedException ex) {
                                    updateStatus("Error installing qmake: " + ex.getMessage());
                                }
                            }
                        } catch (IOException | InterruptedException ex) {
                            updateStatus("Error checking qmake: " + ex.getMessage());
                        }
                        // checking g++
                        try{
                            pb = new ProcessBuilder("g++", "-v");
                            process = pb.start();
                            exitCode = process.waitFor();
                            int android_status=0;
                            if(exitCode == 0){
                                updateStatus("g++ is already installed");
                                status_all=2;
                            }
                            else{
                                updateStatus("g++ needs to be installed");
                                try{
                                    if(os == "Linux"){
                                        pb = new ProcessBuilder("xterm", "-e", "sudo apt install g++ -y");
                                    }
                                    else if(os == "Windows"){
                                        pb = new ProcessBuilder("powershell", "-Command", "Start-Process powershell -Verb runAs -ArgumentList 'winget install -e --id MinGW.Mingw'");
                                    }
                                    else if(os == "MacOS"){
                                        pb = new ProcessBuilder("bash", "-c", "xcode-select --install");
                                    }
                                    else if(os == "Android"){
                                        android_status=1;
                                        pb = new ProcessBuilder("su", "-c", "apt install clang");
                                    }
                                    process = pb.start();
                                    exitCode = process.waitFor();
                                    if(android_status == 1 && exitCode == 0){
                                        updateStatus("installation finished");
                                        status_all=2;
                                    }
                                    else if(android_status == 1 && exitCode != 0){
                                        updateStatus("trying no root method");
                                        pb = new ProcessBuilder("bash","-c","apt install clang -y");
                                        process = pb.start();
                                        exitCode = process.waitFor();
                                        if(exitCode == 0){
                                            updateStatus("installation finished");
                                            status_all=2;
                                        }
                                        else{
                                            updateStatus("installation failed");
                                        }
                                    }
                                    else if(android_status == 0 && exitCode == 0){
                                        updateStatus("g++ installation finished");
                                        status_all=2;
                                    }
                                    else{
                                        updateStatus("g++ installation failed");
                                    }
                                }
                                catch (IOException | InterruptedException ex) {
                                    updateStatus("Error installing g++: " + ex.getMessage());
                                }
                            }
                        }
                        catch (IOException | InterruptedException ex) {
                            updateStatus("Error checking g++: " + ex.getMessage());
                        }
                        // trying to execute qmake to beuild the application
                        try{
                            updateStatus("bulding application");
                            exitCode=0;
                            if(os == "Linux"){
                                pb = new ProcessBuilder("xterm", "-e", "qmake format_ext4_or_fat32.pro");
                                process = pb.start();
                                exitCode = process.waitFor();
                            }
                            else if(os == "Windows"){
                                pb = new ProcessBuilder("powershell", "-Command", "Start-Process powershell -Verb runAs -ArgumentList 'qmake format_ext4_or_fat32.pro'");
                                process = pb.start();
                                exitCode = process.waitFor();
                            }
                            else if(os == "MacOS"){
                                pb = new ProcessBuilder("osascript", "-e", "tell application \"Terminal\" to do script \"qmake format_ext4_or_fat32.pro\"");
                                process = pb.start();
                                exitCode = process.waitFor();
                            }
                            else if(os == "Android"){
                                pb = new ProcessBuilder("bash", "-c", "qmake format_ext4_or_fat32.pro");
                                process = pb.start();
                                exitCode = process.waitFor();
                            }
                            if(exitCode == 0){
                                // making the application, finalizing the building
                                updateStatus("finished building application");
                                status_all=3;
                            }
                            else{
                                updateStatus("failed to build application");
                            }
                        }
                        catch (IOException | InterruptedException ex) {
                            updateStatus("Error building the application: " + ex.getMessage());
                        }
                        updateStatus("Finalizing...");
                        if(status_all==3){
                            try{
                                exitCode=0;
                                if(os == "Linux"){
                                    pb = new ProcessBuilder("xterm", "-e", "make");
                                    process = pb.start();
                                    exitCode = process.waitFor();
                                }
                                else if(os == "Windows"){
                                    pb = new ProcessBuilder("cmd", "/c", "mingw32-make");
                                    process = pb.start();
                                    exitCode = process.waitFor();
                                }
                                else if(os ==  "MacOS"){
                                    pb = new ProcessBuilder("bash", "-c", "make");
                                    process = pb.start();
                                    exitCode = process.waitFor();
                                }
                                else if(os == "Android"){
                                    pb = new ProcessBuilder("bash", "-c", "make");
                                    process = pb.start();
                                    exitCode = process.waitFor();
                                }
                                if(exitCode==0){
                                    status_all=4;
                                }
                            }
                            catch (IOException | InterruptedException ex) {
                                updateStatus("Error building the application: " + ex.getMessage());
                            }
                            if(status_all==4){
                                Thread.sleep(1000);  // Simulate time delay for the step
                                updateStatus("Installation completed successfully!");  // Done message
                            }
                            else{
                                updateStatus("Installation failed");
                            }
                        }
                    } catch (InterruptedException ex) {
                        updateStatus("Installation interrupted.");  // Handle thread interruption
                    } finally {
                        // Re-enable the button on the Swing event thread
                        SwingUtilities.invokeLater(() -> installButton.setEnabled(true));
                    }
                }
            }).start();
        }

        // Helper method to update the status bar text safely on the Swing thread
        private void updateStatus(String message) {
            SwingUtilities.invokeLater(() -> statusBar.setText(message));
        }
    }

    public static void main(String[] args) {
        // Schedule GUI creation on the Swing event dispatch thread
        SwingUtilities.invokeLater(() -> {
            SimpleInstaller installer = new SimpleInstaller();
            installer.setLocationRelativeTo(null); // Center window on screen
            installer.setVisible(true);             // Show the window
        });
    }
}
