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
        Machine machine = new Machine();
        machine.ReadMem();
        machine.close();

        resp.getWriter().println("readmem");

        if(req.getParameter("size").equals("allmem")) {
            int k = 0;
            for(int i=0;i<32768*2;i++) {
                resp.getWriter().printf("%d: 0x%x", i, machine.buff.get(i) & 0xff);
                if (machine.buff.get(i) != 0x00) resp.getWriter().printf(" Not zero!");
                resp.getWriter().printf("\n");
                if ((i + 1) % 4 == 0){
                    resp.getWriter().printf("--- %d ---\n", k+1);
                    k++;
                }
            }
        }else{
            int size = Integer.valueOf(req.getParameter("size"));
            for(int i=0;i<size;i++) {
                resp.getWriter().println(machine.memory[i] & 0xffffffffl);
            }
        }
    }
}
