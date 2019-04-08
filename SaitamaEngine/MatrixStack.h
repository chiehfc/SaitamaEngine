#pragma once
#include <stack>

using namespace DirectX::SimpleMath;

class MatrixStack
{
private:

    Matrix *m_base;		// matrix stack
    Matrix *m_sp;			// top of stack
    int m_stackSize;	// size of stack
    int m_pos;		// current pos of top

public:
    MatrixStack(int size);	// size = depth of stack
    MatrixStack();			    // default size = 8
    ~MatrixStack();
    
    // stack management
    void Reset();		  // Reset to 1, identity on top
    void Push();
    void PushIdentity();	// Push stack, tos = ident
    void Pop();


    // Get the top of stack
    void Get(Matrix& m) const
    {
        m = *m_sp;
    }

    Matrix*	GetTop() const
    {
        return m_sp;
    }

    int   GetDepth() const
    {
        return m_pos;
    }

    int		GetStackSize()
    {
        return m_stackSize;
    }

    void Load(const Matrix& m)
    {
        *m_sp = m;
    }

    void PreMult(const Matrix& m)
    {
        Matrix matrix(*m_sp);
        *m_sp = m * matrix;
    }

    void PostMult(const Matrix& m)
    {
        Matrix matrix(*m_sp);
        *m_sp = matrix * m;
    }
    
    void PushLoad(const Matrix& m)
    {
        Push(); *m_sp = m;
    }

    void PushMult(const Matrix& m)
    {
        Push(); PreMult(m);
    }
};

