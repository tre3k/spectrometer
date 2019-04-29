/* ReadMem.java (c) Kirill Pshenichnyi 2019

 */

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

public class ReadMem extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ExecReadMem(req,resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ExecReadMem(req,resp);
    }

    public void ExecReadMem(HttpServletRequest req, HttpServletResponse resp) throws ServletException,IOException{
        int size = Integer.valueOf(req.getParameter("size"));
        Machine machine = new Machine();
        machine.ReadMem();
        machine.close();


        for(int i=0;i<size;i++){
            resp.getWriter().println(machine.memory[i]+";");
        }


    }
}
